#!/bin/bash

# Script to discover HDF5 and MPI installations via Spack and generate interfaces.
# This script will:
# 1. Find all available HDF5 and MPI packages installed with Spack  
# 2. Allow user to select which ones to use
# 3. Build argument lists for generate_interfaces.py
# 4. Call generate_interfaces.py with the selected packages

set -e  # Exit on any error

# Global variables
declare -a HDF5_NAMES
declare -a HDF5_VERSIONS  
declare -a HDF5_PREFIXES
declare -a HDF5_HEADER_PATHS
declare -a MPI_NAMES
declare -a MPI_VERSIONS
declare -a MPI_PREFIXES
declare -a MPI_HEADER_PATHS
declare -a MPI_IMPLEMENTATIONS  # Store detected MPI implementation type
DRY_RUN=false
QUIET_MODE=false
SELECT_ALL=false
SPACK_ONLY=false
VERBOSE_MODE=false

# Function to display usage
usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --dry-run     Show the command that would be run without executing it"
    echo "  --quiet       Quiet mode for CI - no interactive prompts, auto-continues with warnings"
    echo "  --verbose     Verbose mode - show detailed debug information"
    echo "  --select-all  Automatically select all found HDF5 and MPI versions"
    echo "  --spack-only  Only discover packages from loaded Spack modules"
    echo "  -h, --help    Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                     # Interactive mode"
    echo "  $0 --select-all        # Select all packages automatically"
    echo "  $0 --quiet --dry-run   # CI mode with dry run"
    echo "  $0 --verbose           # Show debug information"
    echo "  $0 --spack-only        # Only use loaded Spack modules"
    echo ""
    exit 1
}

# Parse command line arguments
parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            --dry-run)
                DRY_RUN=true
                shift
                ;;
            --quiet)
                QUIET_MODE=true
                shift
                ;;
            --verbose)
                VERBOSE_MODE=true
                shift
                ;;
            --select-all)
                SELECT_ALL=true
                shift
                ;;
            --spack-only)
                SPACK_ONLY=true
                shift
                ;;
            -h|--help)
                usage
                ;;
            *)
                echo "Unknown option: $1"
                usage
                ;;
        esac
    done
}

# Function to print debug messages only in verbose mode
debug() {
    if [[ "$VERBOSE_MODE" == true ]]; then
        echo "DEBUG: $*" >&2
    fi
}

# Progress bar functions
show_progress() {
    local current=$1
    local total=$2
    local message="$3"
    local width=50
    local percentage=$((current * 100 / total))
    local filled=$((current * width / total))
    local empty=$((width - filled))
    
    printf "\r%s [" "$message"
    printf "%0.s=" $(seq 1 "$filled")
    if [[ $filled -lt $width ]]; then
        printf ">"
        printf "%0.s " $(seq 1 $((empty - 1)))
    fi
    printf "] %d%%" "$percentage"
    
    if [[ $current -eq $total ]]; then
        echo ""
    fi
}

update_discovery_status() {
    local phase="$1"
    local details="$2"
    local current_step="$3"
    local total_steps="$4"
    
    if [[ "$VERBOSE_MODE" == true ]]; then
        if [[ -n "$current_step" && -n "$total_steps" ]]; then
            echo "Step $current_step/$total_steps: $phase $details"
        else
            echo "$phase: $details"
        fi
    else
        # Show progress with completion tracking
        if [[ -n "$current_step" && -n "$total_steps" ]]; then
            printf "\rDiscovering packages... (%s/%s completed) %s                    " "$current_step" "$total_steps" "$phase"
        else
            printf "\rDiscovering packages... %s                    " "$phase"
        fi
    fi
}

# Global progress tracking variables
DISCOVERY_STEPS=0
CURRENT_STEP=0

# Function to initialize discovery progress
init_discovery_progress() {
    DISCOVERY_STEPS=0
    CURRENT_STEP=0
    
    # Count discovery steps based on available package managers
    if check_spack &>/dev/null; then
        DISCOVERY_STEPS=$((DISCOVERY_STEPS + 2))  # HDF5 + MPI via Spack
    fi
    if check_modules &>/dev/null; then
        DISCOVERY_STEPS=$((DISCOVERY_STEPS + 2))  # HDF5 + MPI via modules
    fi
    # Always add system discovery
    DISCOVERY_STEPS=$((DISCOVERY_STEPS + 2))  # HDF5 + MPI via system
}

# Function to update step progress
update_step_progress() {
    local phase_name="$1"
    CURRENT_STEP=$((CURRENT_STEP + 1))
    update_discovery_status "$phase_name" "" "$CURRENT_STEP" "$DISCOVERY_STEPS"
}

# Function to check if spack is available
check_spack() {
    if ! command -v spack &> /dev/null; then
        echo "Warning: Spack not found in PATH"
        return 1
    fi
    return 0
}

# Function to check if modules are available
check_modules() {
    if ! command -v module &> /dev/null; then
        echo "Warning: Module system not found in PATH"
        return 1
    fi
    return 0
}

# Function to check for system packages
check_system_packages() {
    # Always return success since we'll try to find system packages regardless
    return 0
}

# Function to get loaded Spack modules
get_loaded_spack_modules() {
    if ! check_spack; then
        return
    fi
    
    # Get list of loaded modules from Spack
    spack find --loaded 2>/dev/null || true
}

# Function to check if a package is loaded via Spack
is_spack_package_loaded() {
    local package_name="$1"
    local package_version="$2"
    local loaded_modules
    
    loaded_modules=$(get_loaded_spack_modules)
    
    if [[ -z "$loaded_modules" ]]; then
        return 1
    fi
    
    # Check if this specific package@version is in the loaded modules
    if echo "$loaded_modules" | grep -q "${package_name}@${package_version}"; then
        return 0
    fi
    
    return 1
}

# Function to find HDF5 packages
find_hdf5_packages() {
    update_step_progress "Spack HDF5"
    
    local output
    output=$(spack find --format '{name}@{version} {prefix}' hdf5 2>/dev/null || true)
    
    if [[ -z "$output" ]]; then
        debug "No HDF5 output from spack"
        return
    fi
    
    # Process the output using awk and store in array
    local -a pkg_lines
    readarray -t pkg_lines < <(echo "$output" | awk '{
        for (i = 1; i <= NF; i += 2) {
            if (i+1 <= NF && $(i) ~ /@/) {
                print $(i) " " $(i+1)
            }
        }
    }')
    
    # Use associative arrays to track unique versions and header paths
    declare -A seen_versions
    declare -A seen_header_paths
    local index=0
    
    # Process each package line and deduplicate by version and header path
    for pkg_line in "${pkg_lines[@]}"; do
        if [[ -z "$pkg_line" ]]; then
            continue
        fi
        
        local name_version prefix
        read -r name_version prefix <<< "$pkg_line"
        debug "Processing package: $name_version $prefix"
        if [[ "$name_version" == *"@"* ]]; then
            local name version
            name="${name_version%@*}"
            version="${name_version#*@}"
            local header_path="$prefix/include"
            
            # Check if we've already seen this version
            local version_key="${name}@${version}"
            if [[ -n "${seen_versions[$version_key]:-}" ]]; then
                debug "Skipping duplicate HDF5 package: $name@$version (already have ${seen_versions[$version_key]})"
                continue
            fi
            
            # Check if we've already seen this header path
            if [[ -n "${seen_header_paths[$header_path]:-}" ]]; then
                debug "Skipping HDF5 package: $name@$version (header path $header_path already used by ${seen_header_paths[$header_path]})"
                continue
            fi
            
            # If spack-only mode is enabled, check if this package is loaded
            if [[ "$SPACK_ONLY" == true ]]; then
                if ! is_spack_package_loaded "$name" "$version"; then
                    debug "Skipping HDF5 package $name@$version (not loaded in Spack)"
                    continue
                fi
            fi
            
            # Mark this version and header path as seen and store the package info
            seen_versions[$version_key]="$prefix"
            seen_header_paths[$header_path]="$name@$version"
            HDF5_NAMES[index]="$name"
            HDF5_VERSIONS[index]="$version"
            HDF5_PREFIXES[index]="$prefix"
            HDF5_HEADER_PATHS[index]="$header_path"
            debug "Found unique HDF5 package: $name@$version at $prefix (headers: $header_path)"
            index=$((index+1))
        fi
    done
    
    if [[ "$VERBOSE_MODE" == true ]]; then
        echo "Found $index unique HDF5 packages via Spack"
    fi
}

# Function to find HDF5 packages through modules
find_hdf5_modules() {
    update_step_progress "Module HDF5"
    
    if ! check_modules; then
        return
    fi
    
    # Get current array size to continue indexing
    local start_index=${#HDF5_NAMES[@]}
    local index=$start_index
    
    # Use associative arrays to track unique versions and header paths
    declare -A seen_versions
    declare -A seen_header_paths
    
    # Initialize seen_versions and seen_header_paths with existing Spack packages
    for ((i=0; i<start_index; i++)); do
        local version_key="hdf5@${HDF5_VERSIONS[i]}"
        seen_versions[$version_key]="${HDF5_PREFIXES[i]}"
        seen_header_paths["${HDF5_HEADER_PATHS[i]}"]="hdf5@${HDF5_VERSIONS[i]}"
    done
    
    # Try to get module list for HDF5
    local module_output
    module_output=$(module avail hdf5 2>&1 | grep -E "hdf5[/-]" || true)
    
    if [[ -z "$module_output" ]]; then
        debug "No HDF5 modules found"
        return
    fi
    
    # Process module output
    while IFS= read -r line; do
        if [[ -z "$line" ]]; then
            continue
        fi
        
        # Extract module names (handle different module naming conventions)
        local modules
        modules=$(echo "$line" | grep -oE "hdf5[^[:space:]]*" || true)
        
        while IFS= read -r module_name; do
            if [[ -z "$module_name" ]]; then
                continue
            fi
            
            debug "Processing module: $module_name"
            
            # Try to extract version from module name
            local version=""
            if [[ "$module_name" =~ hdf5[/-]([0-9]+\.[0-9]+(\.[0-9]+)?) ]]; then
                version="${BASH_REMATCH[1]}"
            elif [[ "$module_name" =~ hdf5[/-]([0-9]+\.[0-9]+) ]]; then
                version="${BASH_REMATCH[1]}"
            else
                # Try to load module and get version
                if module load "$module_name" 2>/dev/null; then
                    if command -v h5dump &> /dev/null; then
                        version=$(h5dump --version 2>/dev/null | grep -oE "[0-9]+\.[0-9]+\.[0-9]+" | head -1 || echo "")
                    fi
                    module unload "$module_name" 2>/dev/null || true
                fi
                
                if [[ -z "$version" ]]; then
                    debug "Could not determine version for module: $module_name"
                    continue
                fi
            fi
            
            # Check if we've already seen this version
            local version_key="hdf5@${version}"
            if [[ -n "${seen_versions[$version_key]:-}" ]]; then
                debug "Skipping duplicate HDF5 module: $module_name (version $version already found)"
                continue
            fi
            
            # Try to load module and get prefix
            local prefix=""
            if module load "$module_name" 2>/dev/null; then
                # Try different environment variables that might contain the prefix
                if [[ -n "${HDF5_ROOT:-}" ]]; then
                    prefix="$HDF5_ROOT"
                elif [[ -n "${HDF5_DIR:-}" ]]; then
                    prefix="$HDF5_DIR"
                elif [[ -n "${HDF5_HOME:-}" ]]; then
                    prefix="$HDF5_HOME"
                elif command -v h5dump &> /dev/null; then
                    # Try to determine prefix from h5dump location
                    local h5dump_path
                    h5dump_path=$(which h5dump 2>/dev/null || echo "")
                    if [[ -n "$h5dump_path" ]]; then
                        prefix=$(dirname "$(dirname "$h5dump_path")")
                    fi
                fi
                module unload "$module_name" 2>/dev/null || true
            fi
            
            if [[ -z "$prefix" ]]; then
                debug "Could not determine prefix for module: $module_name"
                continue
            fi
            
            # Check if hdf5.h exists
            if [[ ! -f "$prefix/include/hdf5.h" ]]; then
                debug "hdf5.h not found in $prefix/include for module: $module_name"
                continue
            fi
            
            # For modules, trust the module version rather than probing the installation
            # The module system should know what version/interface it's providing
            local module_version="$version"
            local module_version_key="hdf5@${module_version}"
            
            # Optional: detect actual installation version for informational purposes
            local detected_version=""
            if [[ -x "$prefix/bin/h5dump" ]]; then
                detected_version=$("$prefix/bin/h5dump" --version 2>/dev/null | grep -oE "[0-9]+\.[0-9]+\.[0-9]+" | head -1 || echo "")
            fi
            
            if [[ -z "$detected_version" && -f "$prefix/include/H5public.h" ]]; then
                detected_version=$(grep -E "H5_VERS_(MAJOR|MINOR|RELEASE)" "$prefix/include/H5public.h" 2>/dev/null | \
                                 awk '/H5_VERS_MAJOR/ {major=$3} /H5_VERS_MINOR/ {minor=$3} /H5_VERS_RELEASE/ {release=$3} END {print major"."minor"."release}' | \
                                 tr -d '[:space:]' || echo "")
            fi
            
            if [[ -n "$detected_version" && "$detected_version" != "$module_version" ]]; then
                debug "Module $module_name provides version $module_version interface (actual installation: $detected_version)"
            fi
            
            local header_path="$prefix/include"
            
            # Check if we've already seen this module version
            if [[ -n "${seen_versions[$module_version_key]:-}" ]]; then
                debug "Skipping HDF5 module: $module_name (module version $module_version already found at ${seen_versions[$module_version_key]})"
                continue
            fi
            
            # Check if we've already seen this header path
            if [[ -n "${seen_header_paths[$header_path]:-}" ]]; then
                debug "Skipping HDF5 module: $module_name (header path $header_path already used by ${seen_header_paths[$header_path]})"
                continue
            fi
            
            # Mark this module version and header path as seen and store the package info
            seen_versions[$module_version_key]="$prefix"
            seen_header_paths[$header_path]="$module_version"
            HDF5_NAMES[index]="hdf5"
            HDF5_VERSIONS[index]="$module_version"
            HDF5_PREFIXES[index]="$prefix"
            HDF5_HEADER_PATHS[index]="$header_path"
            debug "Found unique HDF5 module: $module_name (version $module_version) at $prefix (headers: $header_path)"
            index=$((index+1))
            
        done <<< "$modules"
    done <<< "$module_output"
    
    local modules_found=$((index - start_index))
    if [[ "$VERBOSE_MODE" == true ]]; then
        echo "Found $modules_found unique HDF5 packages via modules"
    fi
}

# Function to find HDF5 packages through system installations
find_hdf5_system() {
    update_step_progress "System HDF5"
    
    # Get current array size to continue indexing
    local start_index=${#HDF5_NAMES[@]}
    local index=$start_index
    
    # Use associative arrays to track unique versions and header paths
    declare -A seen_versions
    declare -A seen_header_paths
    
    # Initialize seen_versions and seen_header_paths with existing packages
    for ((i=0; i<start_index; i++)); do
        local version_key="hdf5@${HDF5_VERSIONS[i]}"
        seen_versions[$version_key]="${HDF5_PREFIXES[i]}"
        seen_header_paths["${HDF5_HEADER_PATHS[i]}"]="hdf5@${HDF5_VERSIONS[i]}"
    done
    
    # Common system installation paths for HDF5
    local system_prefixes=(
        "/usr"
        "/usr/local" 
        "/opt/hdf5"
        "/opt/local"
        "/usr/lib/x86_64-linux-gnu/hdf5"
        "/usr/lib64/hdf5"
    )
    
    # Additional paths from environment variables
    if [[ -n "${HDF5_ROOT:-}" ]]; then
        system_prefixes+=("$HDF5_ROOT")
    fi
    if [[ -n "${HDF5_DIR:-}" ]]; then
        system_prefixes+=("$HDF5_DIR")
    fi
    if [[ -n "${HDF5_HOME:-}" ]]; then
        system_prefixes+=("$HDF5_HOME")
    fi
    
    # Try to find HDF5 installations using common tools
    if command -v h5dump &> /dev/null; then
        local h5dump_path
        h5dump_path=$(which h5dump 2>/dev/null || echo "")
        if [[ -n "$h5dump_path" ]]; then
            local tool_prefix
            tool_prefix=$(dirname "$(dirname "$h5dump_path")")
            system_prefixes+=("$tool_prefix")
        fi
    fi
    
    # Look for hdf5.pc files to find installations via pkg-config
    if command -v pkg-config &> /dev/null; then
        local pkg_prefix
        pkg_prefix=$(pkg-config --variable=prefix hdf5 2>/dev/null || echo "")
        if [[ -n "$pkg_prefix" ]]; then
            system_prefixes+=("$pkg_prefix")
        fi
    fi
    
    # Check each potential prefix
    for prefix in "${system_prefixes[@]}"; do
        if [[ -z "$prefix" || ! -d "$prefix" ]]; then
            continue
        fi
        
        debug "Checking system prefix: $prefix"
        
        # Look for hdf5.h in include directories
        local header_paths=(
            "$prefix/include"
            "$prefix/include/hdf5/serial"
            "$prefix/include/hdf5/openmpi"
            "$prefix/include/hdf5/mpich"
        )
        
        for header_path in "${header_paths[@]}"; do
            if [[ ! -f "$header_path/hdf5.h" ]]; then
                continue
            fi
            
            debug "Found hdf5.h in: $header_path"
            
            # Try to determine version
            local version=""
            
            # Method 1: Try h5dump if available and in PATH when this prefix is used
            if [[ -x "$prefix/bin/h5dump" ]]; then
                version=$("$prefix/bin/h5dump" --version 2>/dev/null | grep -oE "[0-9]+\.[0-9]+\.[0-9]+" | head -1 || echo "")
            fi
            
            # Method 2: Parse version from hdf5.h header file
            if [[ -z "$version" && -f "$header_path/H5public.h" ]]; then
                version=$(grep -E "H5_VERS_(MAJOR|MINOR|RELEASE)" "$header_path/H5public.h" 2>/dev/null | \
                         awk '/^#define H5_VERS_MAJOR/ {major=$3} /^#define H5_VERS_MINOR/ {minor=$3} /^#define H5_VERS_RELEASE/ {release=$3} END {print major"."minor"."release}' | \
                         tr -d '[:space:]' || echo "")
            fi
            
            # Method 3: Try pkg-config if this looks like a standard installation
            if [[ -z "$version" && "$prefix" == "/usr" ]] && command -v pkg-config &> /dev/null; then
                version=$(pkg-config --modversion hdf5 2>/dev/null || echo "")
            fi
            
            # Method 4: Use system package manager info
            if [[ -z "$version" ]]; then
                if command -v dpkg-query &> /dev/null; then
                    version=$(dpkg-query -W -f='${Version}' libhdf5-dev 2>/dev/null | cut -d'-' -f1 || echo "")
                elif command -v rpm &> /dev/null; then
                    version=$(rpm -q --queryformat '%{VERSION}' hdf5-devel 2>/dev/null || echo "")
                fi
            fi
            
            # Fallback: use a generic version if we can't determine it
            if [[ -z "$version" ]]; then
                version="system"
            fi
            
            debug "Determined version: $version for prefix: $prefix"
            
            # Check if we've already seen this version
            local version_key="hdf5@${version}"
            if [[ -n "${seen_versions[$version_key]:-}" ]]; then
                debug "Skipping duplicate HDF5 system package: $prefix (version $version already found)"
                break  # Break from header_paths loop, continue with next prefix
            fi
            
            # Check if we've already seen this header path
            if [[ -n "${seen_header_paths[$header_path]:-}" ]]; then
                debug "Skipping HDF5 system package: $prefix (header path $header_path already used by ${seen_header_paths[$header_path]})"
                break  # Break from header_paths loop, continue with next prefix
            fi
            
            # Mark this version and header path as seen and store the package info
            seen_versions[$version_key]="$prefix"
            seen_header_paths[$header_path]="hdf5@$version"
            HDF5_NAMES[index]="hdf5"
            HDF5_VERSIONS[index]="$version"
            HDF5_PREFIXES[index]="$prefix"
            HDF5_HEADER_PATHS[index]="$header_path"
            debug "Found unique HDF5 system package: version $version at $prefix (headers: $header_path)"
            index=$((index+1))
            break  # Found valid installation at this prefix, no need to check other header paths
        done
    done
    
    local system_found=$((index - start_index))
    if [[ "$VERBOSE_MODE" == true ]]; then
        echo "Found $system_found unique HDF5 packages via system installation"
    fi
}

# Function to detect MPI implementation type from various sources
detect_mpi_implementation() {
    local mpi_name="$1"
    local mpi_prefix="$2"
    local header_path="$3"
    
    # Method 1: Direct name mapping for known implementations
    case "$mpi_name" in
        openmpi*)
            echo "openmpi"
            return
            ;;
        mpich*)
            echo "mpich"
            return
            ;;
        mvapich*|mvapich2*)
            echo "mvapich"
            return
            ;;
        intel-mpi*|intelmpi*)
            echo "mpich"  # Intel MPI is MPICH-based
            return
            ;;
        cray-mpich*|craympich*)
            echo "craympich"
            return
            ;;
    esac
    
    # Method 2: Check for implementation-specific binaries
    if [[ -n "$mpi_prefix" ]]; then
        # Check for OpenMPI-specific binaries
        if [[ -x "$mpi_prefix/bin/ompi_info" ]] || [[ -x "$mpi_prefix/bin/orterun" ]]; then
            debug "Detected OpenMPI via ompi_info/orterun"
            echo "openmpi"
            return
        fi
        
        # Check for MPICH-specific binaries
        if [[ -x "$mpi_prefix/bin/mpichversion" ]]; then
            debug "Detected MPICH via mpichversion"
            echo "mpich"
            return
        fi
        
        # Check for MVAPICH-specific patterns
        if [[ -x "$mpi_prefix/bin/mpiname" ]]; then
            local mpiname_output
            mpiname_output=$("$mpi_prefix/bin/mpiname" 2>/dev/null || echo "")
            if [[ "$mpiname_output" == *"MVAPICH"* ]]; then
                debug "Detected MVAPICH via mpiname"
                echo "mvapich"
                return
            fi
        fi
    fi
    
    # Method 3: Check mpi.h header for implementation-specific defines
    if [[ -f "$header_path/mpi.h" ]]; then
        # Check for OpenMPI
        if grep -q "OMPI_MAJOR_VERSION\|Open MPI" "$header_path/mpi.h" 2>/dev/null; then
            debug "Detected OpenMPI via mpi.h defines"
            echo "openmpi"
            return
        fi
        
        # Check for MPICH
        if grep -q "MPICH_VERSION\|MPICH_NAME" "$header_path/mpi.h" 2>/dev/null; then
            # Further check if it's Cray MPICH
            if grep -q "CRAY\|cray" "$header_path/mpi.h" 2>/dev/null; then
                debug "Detected Cray-MPICH via mpi.h defines"
                echo "craympich"
                return
            fi
            # Check if it's MVAPICH
            if grep -q "MVAPICH" "$header_path/mpi.h" 2>/dev/null; then
                debug "Detected MVAPICH via mpi.h defines"
                echo "mvapich"
                return
            fi
            debug "Detected MPICH via mpi.h defines"
            echo "mpich"
            return
        fi
        
        # Check for MVAPICH
        if grep -q "MVAPICH" "$header_path/mpi.h" 2>/dev/null; then
            debug "Detected MVAPICH via mpi.h defines"
            echo "mvapich"
            return
        fi
    fi
    
    # Method 4: Check mpirun/mpiexec output
    if [[ -n "$mpi_prefix" ]]; then
        for cmd in "mpirun" "mpiexec"; do
            if [[ -x "$mpi_prefix/bin/$cmd" ]]; then
                local version_output
                version_output=$("$mpi_prefix/bin/$cmd" --version 2>&1 || echo "")
                
                if [[ "$version_output" == *"Open MPI"* ]]; then
                    debug "Detected OpenMPI via $cmd --version"
                    echo "openmpi"
                    return
                elif [[ "$version_output" == *"MPICH"* ]]; then
                    if [[ "$version_output" == *"Cray"* ]] || [[ "$version_output" == *"cray"* ]]; then
                        debug "Detected Cray-MPICH via $cmd --version"
                        echo "craympich"
                        return
                    elif [[ "$version_output" == *"MVAPICH"* ]]; then
                        debug "Detected MVAPICH via $cmd --version"
                        echo "mvapich"
                        return
                    else
                        debug "Detected MPICH via $cmd --version"
                        echo "mpich"
                        return
                    fi
                elif [[ "$version_output" == *"MVAPICH"* ]]; then
                    debug "Detected MVAPICH via $cmd --version"
                    echo "mvapich"
                    return
                fi
            fi
        done
    fi
    
    # Method 5: Check prefix path patterns
    if [[ -n "$mpi_prefix" ]]; then
        if [[ "$mpi_prefix" == *"openmpi"* ]] || [[ "$mpi_prefix" == *"open-mpi"* ]]; then
            debug "Detected OpenMPI via path pattern"
            echo "openmpi"
            return
        elif [[ "$mpi_prefix" == *"cray"* ]] && [[ "$mpi_prefix" == *"mpich"* ]]; then
            debug "Detected Cray-MPICH via path pattern"
            echo "craympich"
            return
        elif [[ "$mpi_prefix" == *"mvapich"* ]]; then
            debug "Detected MVAPICH via path pattern"
            echo "mvapich"
            return
        elif [[ "$mpi_prefix" == *"mpich"* ]]; then
            debug "Detected MPICH via path pattern"
            echo "mpich"
            return
        fi
    fi
    
    # Default: return generic MPI (will not generate implementation-specific macros)
    debug "Could not detect specific MPI implementation, using generic 'mpi'"
    echo "mpi"
}

# Function to find MPI packages
find_mpi_packages() {
    update_step_progress "Spack MPI"
    
    local mpi_packages=("openmpi" "mpich" "intel-mpi" "mvapich2")
    # Use associative arrays to track unique versions and header paths across all MPI implementations
    declare -A seen_versions
    declare -A seen_header_paths
    local index=0
    
    for mpi_pkg in "${mpi_packages[@]}"; do
        debug "Checking $mpi_pkg..."
        local output
        output=$(spack find --format '{name}@{version} {prefix}' "$mpi_pkg" 2>/dev/null || true)
        
        if [[ -z "$output" ]]; then
            debug "No $mpi_pkg packages found"
            continue
        fi
        
        # Process the output using awk and store in array
        local -a pkg_lines
        readarray -t pkg_lines < <(echo "$output" | awk '{
            for (i = 1; i <= NF; i += 2) {
                if (i+1 <= NF && $(i) ~ /@/) {
                    print $(i) " " $(i+1)
                }
            }
        }')
        
        # Process each package line and deduplicate by name@version
        for pkg_line in "${pkg_lines[@]}"; do
            if [[ -z "$pkg_line" ]]; then
                continue
            fi
            
            local name_version prefix
            read -r name_version prefix <<< "$pkg_line"
            
            if [[ "$name_version" == *"@"* ]]; then
                local name version
                name="${name_version%@*}"
                version="${name_version#*@}"
                local header_path="$prefix/include"
                
                # Check if we've already seen this name@version combination
                local version_key="${name}@${version}"
                if [[ -n "${seen_versions[$version_key]:-}" ]]; then
                    debug "Skipping duplicate MPI package: $name@$version (already have ${seen_versions[$version_key]})"
                    continue
                fi
                
                # Check if we've already seen this header path
                if [[ -n "${seen_header_paths[$header_path]:-}" ]]; then
                    debug "Skipping MPI package: $name@$version (header path $header_path already used by ${seen_header_paths[$header_path]})"
                    continue
                fi
                
                # If spack-only mode is enabled, check if this package is loaded
                if [[ "$SPACK_ONLY" == true ]]; then
                    if ! is_spack_package_loaded "$name" "$version"; then
                        debug "Skipping MPI package $name@$version (not loaded in Spack)"
                        continue
                    fi
                fi
                
                # Mark this version and header path as seen and store the package info
                seen_versions[$version_key]="$prefix"
                seen_header_paths[$header_path]="$name@$version"
                MPI_NAMES[index]="$name"
                MPI_VERSIONS[index]="$version"
                MPI_PREFIXES[index]="$prefix"
                MPI_HEADER_PATHS[index]="$header_path"
                # Detect MPI implementation
                MPI_IMPLEMENTATIONS[index]=$(detect_mpi_implementation "$name" "$prefix" "$header_path")
                debug "Found unique MPI package: $name@$version at $prefix (headers: $header_path, impl: ${MPI_IMPLEMENTATIONS[index]})"
                index=$((index+1))
            fi
        done
    done
    
    if [[ "$VERBOSE_MODE" == true ]]; then
        echo "Found $index unique MPI packages via Spack"
    fi
}

# Function to find MPI packages through modules
find_mpi_modules() {
    update_step_progress "Module MPI"
    
    if ! check_modules; then
        return
    fi
    
    # Get current array size to continue indexing
    local start_index=${#MPI_NAMES[@]}
    local index=$start_index
    
    # Use associative arrays to track unique versions and header paths
    declare -A seen_versions
    declare -A seen_header_paths
    
    # Initialize seen_versions and seen_header_paths with existing Spack packages
    for ((i=0; i<start_index; i++)); do
        local version_key="${MPI_NAMES[i]}@${MPI_VERSIONS[i]}"
        seen_versions[$version_key]="${MPI_PREFIXES[i]}"
        seen_header_paths["${MPI_HEADER_PATHS[i]}"]="${MPI_NAMES[i]}@${MPI_VERSIONS[i]}"
    done
    
    # Common MPI module patterns
    local mpi_patterns=("openmpi" "mpich" "intel-mpi" "mvapich2" "mpi" "cray-mpich")
    
    for pattern in "${mpi_patterns[@]}"; do
        debug "Checking for $pattern modules..."
        
        # Try to get module list
        local module_output
        module_output=$(module avail "$pattern" 2>&1 | grep -E "${pattern}[/-]" || true)
        
        if [[ -z "$module_output" ]]; then
            debug "No $pattern modules found"
            continue
        fi
        
        # Process module output
        while IFS= read -r line; do
            if [[ -z "$line" ]]; then
                continue
            fi
            
            # Extract module names
            local modules
            modules=$(echo "$line" | grep -oE "${pattern}[^[:space:]]*" || true)
            
            while IFS= read -r module_name; do
                if [[ -z "$module_name" ]]; then
                    continue
                fi
                
                debug "Processing MPI module: $module_name"
                
                # Extract MPI implementation name and version
                local mpi_name="$pattern"
                local version=""
                
                # For cray-mpich, extract version from module name first
                # The module name format is cray-mpich/VERSION (e.g., cray-mpich/9.0.1)
                if [[ "$pattern" == "cray-mpich" ]]; then
                    if [[ "$module_name" =~ cray-mpich[/-]([0-9]+\.[0-9]+\.[0-9]+) ]]; then
                        version="${BASH_REMATCH[1]}"
                    elif [[ "$module_name" =~ cray-mpich[/-]([0-9]+\.[0-9]+) ]]; then
                        version="${BASH_REMATCH[1]}"
                    fi
                elif [[ "$module_name" =~ ${pattern}[/-]([0-9]+\.[0-9]+(\.[0-9]+)?) ]]; then
                    version="${BASH_REMATCH[1]}"
                elif [[ "$module_name" =~ ${pattern}[/-]([0-9]+\.[0-9]+) ]]; then
                    version="${BASH_REMATCH[1]}"
                else
                    # Try to load module and get version
                    if module load "$module_name" 2>/dev/null; then
                        if command -v mpirun &> /dev/null; then
                            # Try different ways to get MPI version
                            version=$(mpirun --version 2>/dev/null | grep -oE "[0-9]+\.[0-9]+(\.[0-9]+)?" | head -1 || echo "")
                            if [[ -z "$version" ]] && command -v mpiexec &> /dev/null; then
                                version=$(mpiexec --version 2>/dev/null | grep -oE "[0-9]+\.[0-9]+(\.[0-9]+)?" | head -1 || echo "")
                            fi
                        fi
                        module unload "$module_name" 2>/dev/null || true
                    fi
                    
                    if [[ -z "$version" ]]; then
                        debug "Could not determine version for MPI module: $module_name"
                        continue
                    fi
                fi
                
                # Check if we've already seen this name@version combination
                local version_key="${mpi_name}@${version}"
                if [[ -n "${seen_versions[$version_key]:-}" ]]; then
                    debug "Skipping duplicate MPI module: $module_name (${mpi_name}@${version} already found)"
                    continue
                fi
                
                # Try to load module and get prefix
                local prefix=""
                if module load "$module_name" 2>/dev/null; then
                    # Try different environment variables that might contain the prefix
                    # For cray-mpich, check CRAY_MPICH_DIR first
                    if [[ "$mpi_name" == "cray-mpich" && -n "${CRAY_MPICH_DIR:-}" ]]; then
                        prefix="$CRAY_MPICH_DIR"
                    # Also check CRAY_MPICH_DIR for mpich pattern (cray-mpich can be exposed as mpich)
                    elif [[ "$mpi_name" == "mpich" && -n "${CRAY_MPICH_DIR:-}" ]]; then
                        prefix="$CRAY_MPICH_DIR"
                    elif [[ -n "${MPI_ROOT:-}" ]]; then
                        prefix="$MPI_ROOT"
                    elif [[ -n "${MPI_DIR:-}" ]]; then
                        prefix="$MPI_DIR"
                    elif [[ -n "${MPI_HOME:-}" ]]; then
                        prefix="$MPI_HOME"
                    elif [[ -n "${MPICH_ROOT:-}" ]]; then
                        prefix="$MPICH_ROOT"
                    elif [[ -n "${OPENMPI_ROOT:-}" ]]; then
                        prefix="$OPENMPI_ROOT"
                    elif command -v mpirun &> /dev/null; then
                        # Try to determine prefix from mpirun location
                        local mpirun_path
                        mpirun_path=$(which mpirun 2>/dev/null || echo "")
                        if [[ -n "$mpirun_path" ]]; then
                            prefix=$(dirname "$(dirname "$mpirun_path")")
                        fi
                    fi
                    module unload "$module_name" 2>/dev/null || true
                fi
                
                if [[ -z "$prefix" ]]; then
                    debug "Could not determine prefix for MPI module: $module_name"
                    continue
                fi
                
                # Check if mpi.h exists
                if [[ ! -f "$prefix/include/mpi.h" ]]; then
                    debug "mpi.h not found in $prefix/include for module: $module_name"
                    continue
                fi
                
                # Validate that the detected prefix matches the expected MPI implementation
                # This prevents picking up wrong installations from environment views
                local detected_impl
                detected_impl=$(detect_mpi_implementation "$mpi_name" "$prefix" "$prefix/include")
                
                # Skip mpich modules that are actually cray-mpich (detected via path or implementation)
                # These will be found via the cray-mpich pattern instead
                if [[ "$mpi_name" == "mpich" ]]; then
                    if [[ "$prefix" == *"/cray/pe/mpich/"* ]] || [[ "$detected_impl" == "craympich" ]]; then
                        debug "Skipping $module_name: appears to be cray-mpich (will be found via cray-mpich pattern)"
                        continue
                    fi
                fi
                
                if [[ "$mpi_name" == "cray-mpich" && "$detected_impl" != "craympich" ]]; then
                    debug "Skipping $module_name: detected implementation '$detected_impl' at $prefix does not match expected 'craympich'"
                    continue
                fi
                
                # For cray-mpich, also validate that the version in the path matches the module version
                # This prevents picking up wrong installations from environment views
                if [[ "$mpi_name" == "cray-mpich" && "$prefix" =~ /cray/pe/mpich/([0-9]+\.[0-9]+\.[0-9]+) ]]; then
                    local path_version="${BASH_REMATCH[1]}"
                    if [[ "$version" != "$path_version" ]]; then
                        debug "Skipping $module_name: module version '$version' does not match path version '$path_version' at $prefix"
                        continue
                    fi
                fi
                
                # For modules, trust the module name and version rather than probing the installation
                # The module system should know what MPI interface it's providing
                local module_mpi_name="$mpi_name"
                local module_version="$version"
                local module_version_key="${module_mpi_name}@${module_version}"
                
                # Optional: detect actual installation for informational purposes
                local detected_mpi_name="$mpi_name"
                local detected_version="$version"
                
                if [[ -x "$prefix/bin/mpirun" ]]; then
                    local version_output
                    version_output=$("$prefix/bin/mpirun" --version 2>/dev/null || echo "")
                    
                    if [[ -n "$version_output" ]]; then
                        # Detect actual MPI implementation and version
                        if echo "$version_output" | grep -qi "open.*mpi"; then
                            detected_mpi_name="openmpi"
                            local det_ver
                            det_ver=$(echo "$version_output" | grep -oE "[0-9]+\.[0-9]+(\.[0-9]+)?" | head -1 || echo "")
                            if [[ -n "$det_ver" ]]; then
                                detected_version="$det_ver"
                            fi
                        elif echo "$version_output" | grep -qi "mpich"; then
                            detected_mpi_name="mpich"
                            local det_ver
                            det_ver=$(echo "$version_output" | grep -oE "[0-9]+\.[0-9]+(\.[0-9]+)?" | head -1 || echo "")
                            if [[ -n "$det_ver" ]]; then
                                detected_version="$det_ver"
                            fi
                        elif echo "$version_output" | grep -qi "mvapich"; then
                            detected_mpi_name="mvapich2"
                            local det_ver
                            det_ver=$(echo "$version_output" | grep -oE "[0-9]+\.[0-9]+(\.[0-9]+)?" | head -1 || echo "")
                            if [[ -n "$det_ver" ]]; then
                                detected_version="$det_ver"
                            fi
                        fi
                    fi
                fi
                
                if [[ "$detected_version" != "$module_version" || "$detected_mpi_name" != "$module_mpi_name" ]]; then
                    debug "Module $module_name provides ${module_mpi_name}@${module_version} interface (actual installation: ${detected_mpi_name}@${detected_version})"
                fi
                
                local header_path="$prefix/include"
                
                # Check if we've already seen this module version
                if [[ -n "${seen_versions[$module_version_key]:-}" ]]; then
                    debug "Skipping MPI module: $module_name (module ${module_mpi_name}@${module_version} already found at ${seen_versions[$module_version_key]})"
                    continue
                fi
                
                # Check if we've already seen this header path
                if [[ -n "${seen_header_paths[$header_path]:-}" ]]; then
                    debug "Skipping MPI module: $module_name (header path $header_path already used by ${seen_header_paths[$header_path]})"
                    continue
                fi
                
                # Mark this module version and header path as seen and store the package info
                seen_versions[$module_version_key]="$prefix"
                seen_header_paths[$header_path]="${module_mpi_name}@${module_version}"
                MPI_NAMES[index]="$module_mpi_name"
                MPI_VERSIONS[index]="$module_version"
                MPI_PREFIXES[index]="$prefix"
                MPI_HEADER_PATHS[index]="$header_path"
                # Detect MPI implementation
                MPI_IMPLEMENTATIONS[index]=$(detect_mpi_implementation "$module_mpi_name" "$prefix" "$header_path")
                debug "Found unique MPI module: $module_name (${module_mpi_name}@${module_version}) at $prefix (headers: $header_path, impl: ${MPI_IMPLEMENTATIONS[index]})"
                index=$((index+1))
                
            done <<< "$modules"
        done <<< "$module_output"
    done
    
    local modules_found=$((index - start_index))
    if [[ "$VERBOSE_MODE" == true ]]; then
        echo "Found $modules_found unique MPI packages via modules"
    fi
}

# Function to find MPI packages through system installations
find_mpi_system() {
    update_step_progress "System MPI"
    
    # Get current array size to continue indexing
    local start_index=${#MPI_NAMES[@]}
    local index=$start_index
    
    # Use associative arrays to track unique versions and header paths
    declare -A seen_versions
    declare -A seen_header_paths
    
    # Initialize seen_versions and seen_header_paths with existing packages
    for ((i=0; i<start_index; i++)); do
        local version_key="${MPI_NAMES[i]}@${MPI_VERSIONS[i]}"
        seen_versions[$version_key]="${MPI_PREFIXES[i]}"
        seen_header_paths["${MPI_HEADER_PATHS[i]}"]="${MPI_NAMES[i]}@${MPI_VERSIONS[i]}"
    done
    
    # Common system installation paths for MPI
    local system_prefixes=(
        "/usr"
        "/usr/local"
        "/opt/openmpi"
        "/opt/mpich" 
        "/opt/mvapich2"
        "/opt/intel/mpi"
        "/opt/local"
        "/usr/lib/x86_64-linux-gnu/openmpi"
        "/usr/lib/x86_64-linux-gnu/mpich"
        "/usr/lib64/openmpi"
        "/usr/lib64/mpich"
    )
    
    # Add Cray MPICH paths if available
    if [[ -d "/opt/cray/pe/mpich" ]]; then
        # Add all cray-mpich version directories
        for cray_mpich_dir in /opt/cray/pe/mpich/*/ofi/crayclang/*; do
            if [[ -d "$cray_mpich_dir" ]]; then
                system_prefixes+=("$cray_mpich_dir")
            fi
        done
    fi
    
    # Additional paths from environment variables
    local env_vars=("MPI_ROOT" "MPI_DIR" "MPI_HOME" "MPICH_ROOT" "OPENMPI_ROOT" "MVAPICH2_ROOT")
    for var in "${env_vars[@]}"; do
        if [[ -n "${!var:-}" ]]; then
            system_prefixes+=("${!var}")
        fi
    done
    
    # Try to find MPI installations using common tools
    local mpi_commands=("mpirun" "mpiexec" "mpicc")
    for cmd in "${mpi_commands[@]}"; do
        if command -v "$cmd" &> /dev/null; then
            local cmd_path
            cmd_path=$(which "$cmd" 2>/dev/null || echo "")
            if [[ -n "$cmd_path" ]]; then
                local tool_prefix
                tool_prefix=$(dirname "$(dirname "$cmd_path")")
                system_prefixes+=("$tool_prefix")
            fi
        fi
    done
    
    # Look for mpi.pc files to find installations via pkg-config
    if command -v pkg-config &> /dev/null; then
        for mpi_pkg in "mpi" "ompi" "mpich"; do
            local pkg_prefix
            pkg_prefix=$(pkg-config --variable=prefix "$mpi_pkg" 2>/dev/null || echo "")
            if [[ -n "$pkg_prefix" ]]; then
                system_prefixes+=("$pkg_prefix")
            fi
        done
    fi
    
    # Check each potential prefix
    for prefix in "${system_prefixes[@]}"; do
        if [[ -z "$prefix" || ! -d "$prefix" ]]; then
            continue
        fi
        
        debug "Checking MPI system prefix: $prefix"
        
        # Look for mpi.h in include directories
        local header_paths=(
            "$prefix/include"
            "$prefix/include/mpi"
            "$prefix/include/openmpi"
            "$prefix/include/mpich"
        )
        
        for header_path in "${header_paths[@]}"; do
            if [[ ! -f "$header_path/mpi.h" ]]; then
                continue
            fi
            
            debug "Found mpi.h in: $header_path"
            
            # Try to determine MPI implementation and version
            local mpi_name=""
            local version=""
            
            # Method 0: Check for Cray MPICH first (path-based detection)
            # Cray MPICH uses package version in path, not MPICH_VERSION from header
            if [[ "$prefix" == *"/cray/pe/mpich/"* || "$prefix" == *"cray-mpich"* ]]; then
                mpi_name="cray-mpich"
                # Extract version from Cray MPICH path: /opt/cray/pe/mpich/VERSION/...
                # Only match proper semantic versions (all digits): X.Y.Z or X.Y
                if [[ "$prefix" =~ /cray/pe/mpich/([0-9]+\.[0-9]+\.[0-9]+) ]]; then
                    version="${BASH_REMATCH[1]}"
                elif [[ "$prefix" =~ /cray/pe/mpich/([0-9]+\.[0-9]+) ]]; then
                    version="${BASH_REMATCH[1]}"
                else
                    # Skip directories with non-standard version formats (like 8.c.33)
                    debug "Skipping Cray MPICH with non-standard version path: $prefix"
                    continue
                fi
            fi
            
            # Method 1: Try mpirun/mpiexec if available in this prefix
            if [[ -z "$mpi_name" ]]; then
                for cmd in "mpirun" "mpiexec"; do
                    if [[ -x "$prefix/bin/$cmd" ]]; then
                        local version_output
                        version_output=$("$prefix/bin/$cmd" --version 2>/dev/null || echo "")
                        
                        # Detect Open MPI
                        if echo "$version_output" | grep -qi "open.*mpi"; then
                            mpi_name="openmpi"
                            version=$(echo "$version_output" | grep -oE "[0-9]+\.[0-9]+(\.[0-9]+)?" | head -1 || echo "")
                        # Detect MPICH
                        elif echo "$version_output" | grep -qi "mpich"; then
                            mpi_name="mpich"
                            version=$(echo "$version_output" | grep -oE "[0-9]+\.[0-9]+(\.[0-9]+)?" | head -1 || echo "")
                        # Detect MVAPICH2
                        elif echo "$version_output" | grep -qi "mvapich"; then
                            mpi_name="mvapich2"
                            version=$(echo "$version_output" | grep -oE "[0-9]+\.[0-9]+(\.[0-9]+)?" | head -1 || echo "")
                        # Generic MPI
                        else
                            version=$(echo "$version_output" | grep -oE "[0-9]+\.[0-9]+(\.[0-9]+)?" | head -1 || echo "")
                        fi
                        
                        if [[ -n "$version" ]]; then
                            break
                        fi
                    fi
                done
            fi
            
            # Method 2: Try to determine from path patterns
            if [[ -z "$mpi_name" ]]; then
                if [[ "$prefix" == *"openmpi"* || "$header_path" == *"openmpi"* ]]; then
                    mpi_name="openmpi"
                elif [[ "$prefix" == *"mpich"* || "$header_path" == *"mpich"* ]]; then
                    mpi_name="mpich"
                elif [[ "$prefix" == *"mvapich"* || "$header_path" == *"mvapich"* ]]; then
                    mpi_name="mvapich2"
                elif [[ "$prefix" == *"intel"* ]]; then
                    mpi_name="intel-mpi"
                else
                    mpi_name="mpi"
                fi
            fi
            
            # Method 3: Parse version from mpi.h if available
            if [[ -z "$version" && -f "$header_path/mpi.h" ]]; then
                # Try to extract version from MPI_VERSION defines.
                # Match only `#define` lines so comment lines such as
                #   /* MPICH_VERSION is the version string. ... */
                # are not picked up as the version (which would yield "is").
                version=$(grep -E "^[[:space:]]*#[[:space:]]*define[[:space:]]+(OMPI_MAJOR_VERSION|OMPI_MINOR_VERSION|OMPI_RELEASE_VERSION|MPICH_VERSION)[[:space:]]" "$header_path/mpi.h" 2>/dev/null | \
                         awk '
                         /OMPI_MAJOR_VERSION/ {major=$3}
                         /OMPI_MINOR_VERSION/ {minor=$3}
                         /OMPI_RELEASE_VERSION/ {release=$3}
                         /MPICH_VERSION/ {print $3; exit}
                         END {if(major!="" && minor!="" && release!="") print major"."minor"."release}' | \
                         tr -d '[:space:]"' || echo "")
            fi
            
            # Method 4: Try pkg-config
            if [[ -z "$version" && "$prefix" == "/usr" ]] && command -v pkg-config &> /dev/null; then
                for pkg in "ompi" "mpich" "mpi"; do
                    version=$(pkg-config --modversion "$pkg" 2>/dev/null || echo "")
                    if [[ -n "$version" ]]; then
                        break
                    fi
                done
            fi
            
            # Method 5: Use system package manager info
            if [[ -z "$version" ]]; then
                if command -v dpkg-query &> /dev/null; then
                    for pkg in "libopenmpi-dev" "libmpich-dev" "mpi-default-dev"; do
                        version=$(dpkg-query -W -f='${Version}' "$pkg" 2>/dev/null | cut -d'-' -f1 || echo "")
                        if [[ -n "$version" ]]; then
                            break
                        fi
                    done
                elif command -v rpm &> /dev/null; then
                    for pkg in "openmpi-devel" "mpich-devel"; do
                        version=$(rpm -q --queryformat '%{VERSION}' "$pkg" 2>/dev/null || echo "")
                        if [[ -n "$version" ]]; then
                            break
                        fi
                    done
                fi
            fi
            
            # Fallback: use a generic version if we can't determine it
            if [[ -z "$version" ]]; then
                version="system"
            fi
            
            # Default MPI name if still not determined
            if [[ -z "$mpi_name" ]]; then
                mpi_name="mpi"
            fi
            
            debug "Determined MPI: $mpi_name@$version for prefix: $prefix"
            
            # Check if we've already seen this name@version combination
            local version_key="${mpi_name}@${version}"
            if [[ -n "${seen_versions[$version_key]:-}" ]]; then
                debug "Skipping duplicate MPI system package: $prefix (${mpi_name}@${version} already found)"
                break  # Break from header_paths loop, continue with next prefix
            fi
            
            # Check if we've already seen this header path
            if [[ -n "${seen_header_paths[$header_path]:-}" ]]; then
                debug "Skipping MPI system package: $prefix (header path $header_path already used by ${seen_header_paths[$header_path]})"
                break  # Break from header_paths loop, continue with next prefix
            fi
            
            # Mark this version and header path as seen and store the package info
            seen_versions[$version_key]="$prefix"
            seen_header_paths[$header_path]="${mpi_name}@${version}"
            MPI_NAMES[index]="$mpi_name"
            MPI_VERSIONS[index]="$version"
            MPI_PREFIXES[index]="$prefix"
            MPI_HEADER_PATHS[index]="$header_path"
            # Detect MPI implementation
            MPI_IMPLEMENTATIONS[index]=$(detect_mpi_implementation "$mpi_name" "$prefix" "$header_path")
            debug "Found unique MPI system package: ${mpi_name}@${version} at $prefix (headers: $header_path, impl: ${MPI_IMPLEMENTATIONS[index]})"
            index=$((index+1))
            break  # Found valid installation at this prefix, no need to check other header paths
        done
    done
    
    local system_found=$((index - start_index))
    if [[ "$VERBOSE_MODE" == true ]]; then
        echo "Found $system_found unique MPI packages via system installation"
    fi
}

# Function to display available packages
display_packages() {
    echo ""
    echo "============================================================"
    if [[ "$SPACK_ONLY" == true ]]; then
        echo "AVAILABLE PACKAGES (LOADED SPACK MODULES ONLY)"
    else
        echo "AVAILABLE PACKAGES"
    fi
    echo "============================================================"
    
    # Display HDF5 packages
    if [[ ${#HDF5_NAMES[@]} -gt 0 ]]; then
        echo ""
        echo "HDF5 Packages:"
        for i in "${!HDF5_NAMES[@]}"; do
            local status="✗"
            local source="Unknown"
            
            if [[ -f "${HDF5_HEADER_PATHS[i]}/hdf5.h" ]]; then
                status="✓"
            fi
            
            # Try to determine source based on path patterns
            if [[ "${HDF5_PREFIXES[i]}" == *"spack"* ]]; then
                if [[ "$SPACK_ONLY" == true ]] || is_spack_package_loaded "${HDF5_NAMES[i]}" "${HDF5_VERSIONS[i]}"; then
                    source="Spack (loaded)"
                else
                    source="Spack"
                fi
            elif [[ "${HDF5_PREFIXES[i]}" == "/usr" || "${HDF5_PREFIXES[i]}" == "/usr/local" || "${HDF5_PREFIXES[i]}" == *"/opt/"* ]]; then
                source="System"
            elif [[ -n "${MODULESHOME:-}" ]] || command -v module &> /dev/null; then
                source="Module"
            fi
            
            printf "%3d. %s@%s %s [%s]\n" $((i+1)) "${HDF5_NAMES[i]}" "${HDF5_VERSIONS[i]}" "$status" "$source"
            printf "     Path: %s\n" "${HDF5_PREFIXES[i]}"
            printf "     Headers: %s\n" "${HDF5_HEADER_PATHS[i]}"
        done
    else
        echo ""
        echo "No HDF5 packages found."
    fi
    
    # Display MPI packages
    if [[ ${#MPI_NAMES[@]} -gt 0 ]]; then
        echo ""
        echo "MPI Packages:"
        for i in "${!MPI_NAMES[@]}"; do
            local status="✗"
            local source="Unknown"
            
            if [[ -f "${MPI_HEADER_PATHS[i]}/mpi.h" ]]; then
                status="✓"
            fi
            
            # Try to determine source based on path patterns
            if [[ "${MPI_PREFIXES[i]}" == *"spack"* ]]; then
                if [[ "$SPACK_ONLY" == true ]] || is_spack_package_loaded "${MPI_NAMES[i]}" "${MPI_VERSIONS[i]}"; then
                    source="Spack (loaded)"
                else
                    source="Spack"
                fi
            elif [[ "${MPI_PREFIXES[i]}" == "/usr" || "${MPI_PREFIXES[i]}" == "/usr/local" || "${MPI_PREFIXES[i]}" == *"/opt/"* ]]; then
                source="System"
            elif [[ -n "${MODULESHOME:-}" ]] || command -v module &> /dev/null; then
                source="Module"
            fi
            
            # Get implementation
            local impl="${MPI_IMPLEMENTATIONS[i]:-unknown}"
            
            printf "%3d. %s@%s %s [%s] (impl: %s)\n" $((i+1)) "${MPI_NAMES[i]}" "${MPI_VERSIONS[i]}" "$status" "$source" "$impl"
            printf "     Path: %s\n" "${MPI_PREFIXES[i]}"
            printf "     Headers: %s\n" "${MPI_HEADER_PATHS[i]}"
        done
    else
        echo ""
        echo "No MPI packages found."
    fi
    
    echo ""
}

# Function to get user selection for HDF5
get_hdf5_selection() {
    if [[ ${#HDF5_NAMES[@]} -eq 0 ]]; then
        return
    fi
    
    # Handle automatic selection modes
    if [[ "$SELECT_ALL" == true ]]; then
        echo "Auto-selecting all HDF5 packages..."
        SELECTED_HDF5_INDICES=()
        for i in "${!HDF5_NAMES[@]}"; do
            SELECTED_HDF5_INDICES+=("$i")
        done
        return
    fi
    
    if [[ "$QUIET_MODE" == true ]]; then
        echo "Quiet mode: No HDF5 packages selected (use --select-all to select all)"
        SELECTED_HDF5_INDICES=()
        return
    fi
    
    echo "Select HDF5 packages to use:"
    echo "  Enter numbers separated by spaces (e.g., '1 3 5')"
    echo "  Enter 'all' to select all packages"
    echo "  Enter 'none' or press Enter to skip HDF5"
    
    local selection
    while true; do
        read -p "HDF5 selection: " selection
        selection=$(echo "$selection" | tr '[:upper:]' '[:lower:]' | xargs)
        
        if [[ -z "$selection" || "$selection" == "none" ]]; then
            SELECTED_HDF5_INDICES=()
            break
        elif [[ "$selection" == "all" ]]; then
            SELECTED_HDF5_INDICES=()
            for i in "${!HDF5_NAMES[@]}"; do
                SELECTED_HDF5_INDICES+=("$i")
            done
            break
        else
            # Parse numbers
            local valid=true
            local indices=()
            for num in $selection; do
                if [[ "$num" =~ ^[0-9]+$ ]] && [[ $num -ge 1 ]] && [[ $num -le ${#HDF5_NAMES[@]} ]]; then
                    indices+=("$((num-1))")
                else
                    valid=false
                    break
                fi
            done
            
            if [[ "$valid" == true ]]; then
                SELECTED_HDF5_INDICES=("${indices[@]}")
                break
            else
                echo "Invalid selection. Please try again."
            fi
        fi
    done
}

# Function to get user selection for MPI
get_mpi_selection() {
    if [[ ${#MPI_NAMES[@]} -eq 0 ]]; then
        return
    fi
    
    # Handle automatic selection modes
    if [[ "$SELECT_ALL" == true ]]; then
        echo "Auto-selecting all MPI packages..."
        SELECTED_MPI_INDICES=()
        for i in "${!MPI_NAMES[@]}"; do
            SELECTED_MPI_INDICES+=("$i")
        done
        return
    fi
    
    if [[ "$QUIET_MODE" == true ]]; then
        echo "Quiet mode: No MPI packages selected (use --select-all to select all)"
        SELECTED_MPI_INDICES=()
        return
    fi
    
    echo ""
    echo "Select MPI packages to use:"
    echo "  Enter numbers separated by spaces (e.g., '1 2')"
    echo "  Enter 'all' to select all packages"
    echo "  Enter 'none' or press Enter to skip MPI"
    
    local selection
    while true; do
        read -p "MPI selection: " selection
        selection=$(echo "$selection" | tr '[:upper:]' '[:lower:]' | xargs)
        
        if [[ -z "$selection" || "$selection" == "none" ]]; then
            SELECTED_MPI_INDICES=()
            break
        elif [[ "$selection" == "all" ]]; then
            SELECTED_MPI_INDICES=()
            for i in "${!MPI_NAMES[@]}"; do
                SELECTED_MPI_INDICES+=("$i")
            done
            break
        else
            # Parse numbers
            local valid=true
            local indices=()
            for num in $selection; do
                if [[ "$num" =~ ^[0-9]+$ ]] && [[ $num -ge 1 ]] && [[ $num -le ${#MPI_NAMES[@]} ]]; then
                    indices+=("$((num-1))")
                else
                    valid=false
                    break
                fi
            done
            
            if [[ "$valid" == true ]]; then
                SELECTED_MPI_INDICES=("${indices[@]}")
                break
            else
                echo "Invalid selection. Please try again."
            fi
        fi
    done
}

# Function to get clang version
get_clang_version() {
    if command -v clang &> /dev/null; then
        clang --version 2>/dev/null | head -1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -1
    else
        echo ""
    fi
}

# Function to get the correct Python executable
get_python_executable() {
    # Check if we're in a virtual environment
    if [[ -n "$VIRTUAL_ENV" ]]; then
        echo "$VIRTUAL_ENV/bin/python"
    elif command -v python3 &> /dev/null; then
        echo "python3"
    elif command -v python &> /dev/null; then
        echo "python"
    else
        echo ""
    fi
}

# Function to get dynamic MPI version using a compiled C program
get_dynamic_mpi_version() {
    local mpi_prefix="$1"
    local mpi_include_path="$2"
    
    # Create temporary directory for MPI version detection
    local temp_dir
    temp_dir=$(mktemp -d)
    local mpi_version_program="$temp_dir/mpi_version_check"
    local mpi_version_source="$temp_dir/mpi_version_check.c"
    
    # Create the MPI version detection program
    cat > "$mpi_version_source" << 'EOF'
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int version, subversion;
    MPI_Get_version(&version, &subversion);
    printf("%d.%d\n", version, subversion);

    MPI_Finalize();
    return 0;
}
EOF
    
    # Try to find MPI compiler
    local mpi_compiler=""
    local mpi_compilers=("mpicc" "mpic++")
    
    # First try MPI compilers from the specific prefix
    if [[ -n "$mpi_prefix" ]]; then
        for compiler in "${mpi_compilers[@]}"; do
            if [[ -x "$mpi_prefix/bin/$compiler" ]]; then
                mpi_compiler="$mpi_prefix/bin/$compiler"
                break
            fi
        done
    fi
    
    # Fall back to system MPI compilers
    if [[ -z "$mpi_compiler" ]]; then
        for compiler in "${mpi_compilers[@]}"; do
            if command -v "$compiler" &> /dev/null; then
                mpi_compiler="$compiler"
                break
            fi
        done
    fi
    
    # If no MPI compiler found, try regular compiler with manual includes
    if [[ -z "$mpi_compiler" ]]; then
        local regular_compilers=("gcc" "clang" "cc")
        for compiler in "${regular_compilers[@]}"; do
            if command -v "$compiler" &> /dev/null; then
                mpi_compiler="$compiler"
                break
            fi
        done
        
        if [[ -n "$mpi_compiler" && -n "$mpi_include_path" ]]; then
            # Find MPI library path
            local mpi_lib_path=""
            if [[ -n "$mpi_prefix" ]]; then
                if [[ -d "$mpi_prefix/lib" ]]; then
                    mpi_lib_path="$mpi_prefix/lib"
                elif [[ -d "$mpi_prefix/lib64" ]]; then
                    mpi_lib_path="$mpi_prefix/lib64"
                fi
            fi
            
            # Compile with manual flags
            local compile_cmd=("$mpi_compiler" "-I$mpi_include_path" "$mpi_version_source" "-o" "$mpi_version_program")
            if [[ -n "$mpi_lib_path" ]]; then
                compile_cmd+=("-L$mpi_lib_path" "-lmpi")
            else
                compile_cmd+=("-lmpi")
            fi
        else
            debug "No suitable compiler found for MPI version detection"
            rm -rf "$temp_dir"
            echo ""
            return
        fi
    else
        # Use MPI compiler directly
        local compile_cmd=("$mpi_compiler" "$mpi_version_source" "-o" "$mpi_version_program")
    fi
    
    # Compile the program
    debug "Compiling MPI version check program with: ${compile_cmd[*]}"
    if "${compile_cmd[@]}" 2>/dev/null; then
        debug "Successfully compiled MPI version check program"
        
        # Run the program to get version
        local dynamic_version=""
        if [[ -x "$mpi_version_program" ]]; then
            # Try to run with mpirun if available, otherwise run directly
            if command -v mpirun &> /dev/null; then
                dynamic_version=$(timeout 10 mpirun -n 1 "$mpi_version_program" 2>/dev/null || echo "")
            fi
            
            # If mpirun failed or not available, try running directly
            if [[ -z "$dynamic_version" ]]; then
                dynamic_version=$(timeout 10 "$mpi_version_program" 2>/dev/null || echo "")
            fi
        fi
        
        debug "Dynamic MPI version detected: $dynamic_version"
        echo "$dynamic_version"
    else
        debug "Failed to compile MPI version check program"
        echo ""
    fi
    
    # Clean up
    rm -rf "$temp_dir"
}

# Function to check and install matching libclang Python package
check_python_libclang() {
    local clang_version="$1"
    local python_exe
    python_exe=$(get_python_executable)
    
    # Check if python is available
    if [[ -z "$python_exe" ]]; then
        echo "Warning: Python not found, cannot check libclang Python package"
        return
    fi
    
    echo "Using Python: $python_exe"
    
    # Check if libclang is installed via pip
    local installed_libclang
    installed_libclang=$($python_exe -c "
try:
    import importlib.metadata
    print(importlib.metadata.version('libclang'))
except ImportError:
    try:
        import pkg_resources
        dist = pkg_resources.get_distribution('libclang')
        print(dist.version)
    except (ImportError, pkg_resources.DistributionNotFound):
        print('')
except importlib.metadata.PackageNotFoundError:
    print('')
" 2>/dev/null || echo "")
    
    if [[ -n "$installed_libclang" ]]; then
        echo "Found Python libclang package version: $installed_libclang"
        
        # Check if versions match (major.minor)
        if [[ -n "$clang_version" ]]; then
            local clang_major_minor
            clang_major_minor=$(echo "$clang_version" | cut -d. -f1-2)
            local libclang_major_minor
            libclang_major_minor=$(echo "$installed_libclang" | cut -d. -f1-2)
            
            if [[ "$clang_major_minor" != "$libclang_major_minor" ]]; then
                echo "Warning: Version mismatch detected!"
                echo "  System clang version: $clang_version (major.minor: $clang_major_minor)"
                echo "  Python libclang version: $installed_libclang (major.minor: $libclang_major_minor)"
                echo ""
                echo "To fix this, you can run:"
                echo "  $python_exe -m pip install libclang==$clang_major_minor.*"
                echo ""
                
                if [[ "$QUIET_MODE" == true ]]; then
                    echo "Quiet mode: Continuing with version mismatch..."
                else
                    read -p "Do you want to continue anyway? (y/N): " continue_anyway
                    if [[ "$continue_anyway" != "y" && "$continue_anyway" != "Y" ]]; then
                        echo "Exiting. Please install matching libclang version."
                        exit 1
                    fi
                fi
            else
                echo "Python libclang version matches clang version ✓"
            fi
        fi
    else
        if [[ -n "$clang_version" ]]; then
            echo "Python libclang package not found."
            echo "It's recommended to install it for better compatibility:"
            local clang_major_minor
            clang_major_minor=$(echo "$clang_version" | cut -d. -f1-2)
            echo "  $python_exe -m pip install libclang==$clang_major_minor.*"
            echo ""
            
            if [[ "$QUIET_MODE" == true ]]; then
                echo "Quiet mode: Continuing without Python libclang package..."
            else
                read -p "Do you want to continue without Python libclang? (y/N): " continue_anyway
                if [[ "$continue_anyway" != "y" && "$continue_anyway" != "Y" ]]; then
                    echo "Exiting. Please install libclang Python package."
                    exit 1
                fi
            fi
        fi
    fi
}

# Function to find libclang (internal, returns only the path)
_find_libclang_path() {
    local libclang_path=""

    # 0. Explicit override. Spack/module systems (and CI) can set this to
    # avoid relying on filesystem layout heuristics.
    if [[ -n "${LIBCLANG_PATH:-}" && -f "${LIBCLANG_PATH}" ]]; then
        echo "${LIBCLANG_PATH}"
        return
    fi

    # 1. Ask clang itself. `clang -print-file-name=libclang.so` returns the
    # libclang shipped with the active clang toolchain.
    if command -v clang &> /dev/null; then
        local printed
        printed=$(clang -print-file-name=libclang.so 2>/dev/null || true)
        if [[ -n "$printed" && "$printed" != "libclang.so" && -f "$printed" ]]; then
            echo "$printed"
            return
        fi
    fi

    # 2. Try to find libclang based on the currently loaded clang
    if command -v clang &> /dev/null; then
        local clang_path
        clang_path=$(which clang)
        local clang_dir
        clang_dir=$(dirname "$clang_path")
        local clang_root
        clang_root=$(dirname "$clang_dir")

        # Check if libclang.so is in the same installation
        local potential_paths=(
            "$clang_root/lib/libclang.so"
            "$clang_root/lib64/libclang.so"
            "$clang_root/lib/x86_64-linux-gnu/libclang.so"
        )

        for path in "${potential_paths[@]}"; do
            if [[ -f "$path" ]]; then
                libclang_path="$path"
                break
            fi
        done
    fi
    
    # If not found with clang, try common system paths
    if [[ -z "$libclang_path" ]]; then
        local common_paths=(
            "/usr/lib/x86_64-linux-gnu/libclang.so"
            "/usr/lib64/libclang.so"
            "/usr/local/lib/libclang.so"
            "/opt/llvm/lib/libclang.so"
        )
        
        for path in "${common_paths[@]}"; do
            if [[ -f "$path" ]]; then
                libclang_path="$path"
                break
            fi
        done
    fi
    
    # Try to find via pkg-config
    if [[ -z "$libclang_path" ]]; then
        local pkg_libdir
        pkg_libdir=$(pkg-config --variable=libdir libclang 2>/dev/null || true)
        if [[ -n "$pkg_libdir" ]]; then
            local full_path="$pkg_libdir/libclang.so"
            if [[ -f "$full_path" ]]; then
                libclang_path="$full_path"
            fi
        fi
    fi
    
    # Try to find via find command in system directories
    if [[ -z "$libclang_path" ]]; then
        local found_path
        found_path=$(find /usr -name 'libclang.so*' 2>/dev/null | head -1)
        if [[ -n "$found_path" && -f "$found_path" ]]; then
            libclang_path="$found_path"
        fi
    fi
    
    # Check if it's available via Spack
    if [[ -z "$libclang_path" ]]; then
        local spack_llvm
        spack_llvm=$(spack find --format '{prefix}' llvm 2>/dev/null || true)
        if [[ -n "$spack_llvm" ]]; then
            while IFS= read -r line; do
                if [[ -n "$line" ]]; then
                    local candidate="$line/lib/libclang.so"
                    if [[ -f "$candidate" ]]; then
                        libclang_path="$candidate"
                        break
                    fi
                fi
            done <<< "$spack_llvm"
        fi
    fi
    
    # Return the path (could be empty if not found)
    echo "$libclang_path"
}

# Function to find libclang (public interface with verbose output)
find_libclang() {
    local clang_version
    clang_version=$(get_clang_version)
    
    echo "System clang version: ${clang_version:-"not found"}" >&2
    
    # Check Python libclang package compatibility
    if [[ -n "$clang_version" ]]; then
        check_python_libclang "$clang_version" >&2
    fi
    
    # Find the actual libclang path
    local libclang_path
    libclang_path=$(_find_libclang_path)
    
    if [[ -n "$libclang_path" ]]; then
        echo "Found libclang: $libclang_path" >&2
    fi
    
    # Return only the path
    echo "$libclang_path"
}

# Function to display selected packages
display_selected_packages() {
    echo ""
    echo "Selected packages:"
    
    if [[ ${#SELECTED_HDF5_INDICES[@]} -gt 0 ]]; then
        echo "HDF5:"
        for i in "${SELECTED_HDF5_INDICES[@]}"; do
            echo "  - ${HDF5_NAMES[i]}@${HDF5_VERSIONS[i]} (${HDF5_HEADER_PATHS[i]})"
        done
    fi
    
    if [[ ${#SELECTED_MPI_INDICES[@]} -gt 0 ]]; then
        echo "MPI:"
        for i in "${SELECTED_MPI_INDICES[@]}"; do
            local impl="${MPI_IMPLEMENTATIONS[i]:-unknown}"
            echo "  - ${MPI_NAMES[i]}@${MPI_VERSIONS[i]} (${MPI_HEADER_PATHS[i]}) [impl: $impl]"
        done
    fi
}

# Function to build and execute generate_interfaces command
build_and_execute_command() {
    local libclang_path="$1"
    local script_dir
    script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    local generate_script="$script_dir/generate_interfaces.py"
    
    # Get the correct Python executable
    local python_exe
    python_exe=$(get_python_executable)
    if [[ -z "$python_exe" ]]; then
        echo "Error: Python executable not found"
        exit 1
    fi
    
    # Build command array
    local cmd=("$python_exe" "$generate_script" "--libclang-path" "$libclang_path" "--with-tests" "--mpi-layered")
    
    # Add HDF5 arguments
    if [[ ${#SELECTED_HDF5_INDICES[@]} -gt 0 ]]; then
        cmd+=("--hdf5-header-path")
        for i in "${SELECTED_HDF5_INDICES[@]}"; do
            cmd+=("${HDF5_HEADER_PATHS[i]}")
        done
        
        cmd+=("--hdf5-version")
        for i in "${SELECTED_HDF5_INDICES[@]}"; do
            cmd+=("${HDF5_VERSIONS[i]}")
        done
    fi
    
    # Add MPI arguments
    if [[ ${#SELECTED_MPI_INDICES[@]} -gt 0 ]]; then
        cmd+=("--mpi-header-path")
        for i in "${SELECTED_MPI_INDICES[@]}"; do
            cmd+=("${MPI_HEADER_PATHS[i]}")
        done
        
        cmd+=("--mpi-version")
        for i in "${SELECTED_MPI_INDICES[@]}"; do
            # Use package version instead of detected library version
            debug "Using package MPI version: ${MPI_VERSIONS[i]} for ${MPI_NAMES[i]}@${MPI_VERSIONS[i]}"
            cmd+=("${MPI_VERSIONS[i]}")
        done
        
        # Add MPI implementation information
        cmd+=("--mpi-implementation")
        for i in "${SELECTED_MPI_INDICES[@]}"; do
            local impl="${MPI_IMPLEMENTATIONS[i]:-mpi}"
            if [[ "$VERBOSE_MODE" == true ]]; then
                echo "Adding MPI implementation: $impl for ${MPI_NAMES[i]}@${MPI_VERSIONS[i]}"
            fi
            cmd+=("$impl")
        done
    fi
    
    echo ""
    echo "Generate interfaces command:"
    printf '%q ' "${cmd[@]}"
    echo ""
    
    if [[ "$DRY_RUN" == true ]]; then
        echo ""
        echo "Dry run mode - command not executed"
        return
    fi
    
    # Execute command
    echo ""
    echo "Executing generate_interfaces.py..."
    echo "------------------------------------------------------------"
    
    if "${cmd[@]}"; then
        echo "------------------------------------------------------------"
        echo "Interface generation completed successfully!"
    else
        echo ""
        echo "Error executing generate_interfaces.py"
        exit 1
    fi

    # Display generated interface files
    echo ""
    echo "============================================================"
    echo "GENERATED INTERFACE PREVIEWS"
    echo "============================================================"

    # Function to show interface file preview
    show_interface_preview() {
        local file_path="$1"
        local file_type="$2"
        
        if [[ -f "$file_path" ]]; then
            echo ""
            echo "--- $file_type Interface (first 20 lines) ---"
            echo "File: $file_path"
            echo ""
            head -53 "$file_path" | tail -20
            echo ""
            echo "... (file continues)"
        else
            echo ""
            echo "--- $file_type Interface ---"
            echo "File not found: $file_path"
        fi
    }

    # Look for generated interface files in the correct location
    local interface_dir="$script_dir/../include/brahma/interface"

    # Check for MPI interface files (prioritize MPIIO then MPI)
    if [[ ${#SELECTED_MPI_INDICES[@]} -gt 0 ]]; then
        # First try to show mpiio.h
        local mpiio_file="$interface_dir/mpiio.h"
        if [[ -f "$mpiio_file" ]]; then
            show_interface_preview "$mpiio_file" "MPI-IO"
        fi
        
        # Then show mpi.h
        local mpi_file="$interface_dir/mpi.h"
        if [[ -f "$mpi_file" ]]; then
            show_interface_preview "$mpi_file" "MPI"
        fi
    fi

    # Check for HDF5 interface files (only if HDF5 was selected)
    if [[ ${#SELECTED_HDF5_INDICES[@]} -gt 0 ]]; then
        local hdf5_file="$interface_dir/hdf5.h"
        if [[ -f "$hdf5_file" ]]; then
            show_interface_preview "$hdf5_file" "HDF5"
        fi
    fi

    echo ""
    echo "============================================================"
    echo "Interface generation and preview completed!"
    echo "============================================================"
}

# Main function
main() {
    echo "Brahma Interface Generator - Package Discovery"
    echo "============================================================"
    
    # Check what package managers are available
    local has_spack=false
    local has_modules=false
    
    if check_spack; then
        has_spack=true
        echo "Spack found ✓"
    fi
    
    if check_modules; then
        has_modules=true
        echo "Module system found ✓"
    fi
    
    echo "System package discovery enabled ✓"
    
    # Initialize progress tracking
    init_discovery_progress
    
    # Find packages using selected methods
    if [[ "$SPACK_ONLY" == true ]]; then
        if [[ "$VERBOSE_MODE" == true ]]; then
            echo "Spack-only mode: Only discovering loaded Spack packages..."
        fi
        if [[ "$has_spack" == true ]]; then
            if [[ "$VERBOSE_MODE" == true ]]; then
                echo "Discovering loaded Spack packages..."
            fi
            find_hdf5_packages
            find_mpi_packages
        else
            echo ""
            echo "Error: --spack-only specified but Spack is not available"
            exit 1
        fi
    else
        # Find packages using all available methods
        if [[ "$has_spack" == true ]]; then
            if [[ "$VERBOSE_MODE" == true ]]; then
                echo "Discovering Spack packages..."
            fi
            find_hdf5_packages
            find_mpi_packages
        fi
        
        if [[ "$has_modules" == true ]]; then
            if [[ "$VERBOSE_MODE" == true ]]; then
                echo "Discovering module packages..."
            fi
            find_hdf5_modules
            find_mpi_modules
        fi
        
        # Always try system packages
        if [[ "$VERBOSE_MODE" == true ]]; then
            echo "Discovering system packages..."
        fi
        find_hdf5_system
        find_mpi_system
    fi
    
    # Clear the progress line and show summary
    if [[ "$VERBOSE_MODE" != true ]]; then
        printf "\rDiscovering packages... Complete! (%s/%s completed)\n" "$CURRENT_STEP" "$DISCOVERY_STEPS"
    fi
    
    # Show discovery summary
    local total_hdf5=${#HDF5_NAMES[@]}
    local total_mpi=${#MPI_NAMES[@]}
    echo ""
    echo "Discovery Summary:"
    echo "  HDF5 packages found: $total_hdf5"
    echo "  MPI packages found: $total_mpi"
    
    # Check if any packages were found
    if [[ ${#HDF5_NAMES[@]} -eq 0 && ${#MPI_NAMES[@]} -eq 0 ]]; then
        if [[ "$SPACK_ONLY" == true ]]; then
            echo "No HDF5 or MPI packages found in loaded Spack modules."
            echo "Please load HDF5 or MPI packages via Spack:"
            echo "  spack load hdf5"
            echo "  spack load openmpi"
            echo "  # or"
            echo "  spack load mpich"
        else
            echo "No HDF5 or MPI packages found via Spack, modules, or system installation."
            echo "Please ensure HDF5 or MPI packages are installed through one of these methods:"
            echo "  - System package manager (apt, yum, dnf, etc.)"
            echo "  - Spack package manager"
            echo "  - Environment modules"
            echo "  - Manual installation in standard locations (/usr, /usr/local, /opt)"
        fi
        exit 1
    fi
    
    # Display packages
    display_packages
    
    # Get user selections
    declare -a SELECTED_HDF5_INDICES
    declare -a SELECTED_MPI_INDICES
    
    get_hdf5_selection
    get_mpi_selection
    
    # Check if any packages were selected
    if [[ ${#SELECTED_HDF5_INDICES[@]} -eq 0 && ${#SELECTED_MPI_INDICES[@]} -eq 0 ]]; then
        if [[ "$QUIET_MODE" == true ]]; then
            echo "Error: No packages selected in quiet mode. Use --select-all to select all packages."
            exit 1
        else
            echo "No packages selected. Exiting."
            exit 0
        fi
    fi
    
    # Display selected packages
    display_selected_packages
    
    # Find libclang
    echo "Finding libclang..."
    local libclang_path
    libclang_path=$(find_libclang)
    if [[ -z "$libclang_path" ]]; then
        echo ""
        echo "Error: Could not find libclang.so"
        echo "Please install libclang or specify the path manually"
        echo ""
        echo "You can try:"
        echo "  - Install clang and libclang development packages"
        echo "  - Use spack: spack install llvm+clang"
        echo "  - Manually specify the path in the script"
        exit 1
    fi
    
    echo "Using libclang: $libclang_path"
    
    # Build and execute command
    build_and_execute_command "$libclang_path"
}

# Parse arguments and run main
parse_args "$@"
main
