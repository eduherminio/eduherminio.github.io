docker build -t local-eduherminio.github.io .
docker-compose up
REM wsl docker run -p 8080:4000 -v $(pwd):/site --rm -it $(docker build -q .)  # Only works in wsl due to pwd
