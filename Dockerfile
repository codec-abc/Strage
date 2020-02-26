FROM ubuntu:18.04

WORKDIR /app

RUN \
    apt-get update && \
    apt-get -y install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev git libvorbis-dev libogg-dev libsdl2-ttf-dev wget mingw-w64 zip python cmake && \
    git clone https://github.com/emscripten-core/emsdk.git 

# Build this image with
# docker build -t strage ./
# Then start container and hook into it with (adapt path is necessary)
# docker run --rm -it -v D:\Workplace\strage-wasm\Strage:/HostMount strage

# Then type thoses command into the terminal to build the game
# cd /app/emsdk
# ./emsdk install latest
# ./emsdk install latest
# source ./emsdk_env.sh
# cd /HostMount
# emcc ./Sources/*.cpp -I ./Includes -s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_OGG=1 -s USE_VORBIS=1 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s ASSERTIONS=1 -s EXIT_RUNTIME=1 -s ASYNCIFY=1 -s ALLOW_MEMORY_GROWTH=1  -O3 -o build/index.js --preload-file data@


# To do a debug build use instead
# emcc ./Sources/*.cpp -I ./Includes -s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_OGG=1 -s USE_VORBIS=1 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s ASSERTIONS=1 -s EXIT_RUNTIME=1 -s ASYNCIFY=1 -s ALLOW_MEMORY_GROWTH=1  -O3 -o build/index.js --preload-file data@ -g4 --source-map-base localhost

# https://books.google.fr/books?id=sfeaDwAAQBAJ&pg=PA83&lpg=PA83&dq=Hands-On+Game+Development+with+WebAssembly+emcc+sdl&source=bl&ots=QZev7sieNf&sig=ACfU3U2J46bfueYbm-rAOx9y-6lhJEATqA&hl=fr&sa=X&ved=2ahUKEwjw5cvZ-qjkAhVFxYUKHWa4A4gQ6AEwAnoECAkQAQ#v=onepage&q=Hands-On%20Game%20Development%20with%20WebAssembly%20emcc%20sdl&f=false

# emcc ./Sources/*.cpp -I ./Includes -s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_OGG=1 -s USE_VORBIS=1 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s ASSERTIONS=1 -s EXIT_RUNTIME=1 -s EMTERPRETIFY=1 -s EMTERPRETIFY_ASYNC=1 -s ALLOW_MEMORY_GROWTH=1  -O3 -o build/index.js --preload-file data@