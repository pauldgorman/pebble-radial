# Stop and remove existing container to ensure clean state with correct mounts
Write-Host "Stopping any existing pebble-dev container..."
docker rm -f pebble-dev | Out-Null

# Run the container with VNC port mapped and current directory mounted
Write-Host "Starting container..."
docker run -it -p 5900:5900 --name pebble-dev -v "${PWD}:/code" -w /code pebble-watchface:latest /bin/bash /code/launch_sim.sh
