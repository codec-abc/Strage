CPP = g++
CPPFLAGS = -W -Wall -g

PATH_INCLUDES = Includes
PATH_SOURCES = Sources

BINARY = Strage
LIBRARIES = -lSDL2 -lSDL2_ttf
SOURCES = $(shell find $(PATH_SOURCES) -name "*.cpp")

# Program configuration
CPPFLAGS += -DCONFIGURATION_DISPLAY_WIDTH=1024
CPPFLAGS += -DCONFIGURATION_DISPLAY_HEIGHT=768
# For a 60Hz refresh frequency, the period is 1/60 = 0.016ms
CPPFLAGS += -DCONFIGURATION_DISPLAY_REFRESH_PERIOD_MILLISECONDS=16
CPPFLAGS += -DCONFIGURATION_DISPLAY_HUD_LIFE_POINTS_X=10
CPPFLAGS += -DCONFIGURATION_DISPLAY_HUD_LIFE_POINTS_Y=10
# Set which logs to display (error logs are always enabled) :
# 0 : error messages
# 1 : error and information messages
# 2 : error, information and debug messages
# 3 : all previous stuff plus in-game displaying of some meaningful collision rectangles
CPPFLAGS += -DCONFIGURATION_LOG_LEVEL=2
CPPFLAGS += -DCONFIGURATION_PATH_LEVELS=\"Levels\"
CPPFLAGS += -DCONFIGURATION_PATH_TEXTURES=\"Textures\"
CPPFLAGS += -DCONFIGURATION_LEVEL_MAXIMUM_WIDTH=256
CPPFLAGS += -DCONFIGURATION_LEVEL_MAXIMUM_HEIGHT=256
CPPFLAGS += -DCONFIGURATION_LEVEL_BLOCK_SIZE=64

all:
	$(CPP) $(CPPFLAGS) -I$(PATH_INCLUDES) $(SOURCES) $(LIBRARIES) -o $(BINARY)

clean:
	rm -f $(BINARY)
