/** @file AudioManager.cpp
 * See AudioManager.hpp for description.
 * @author Adrien RICCIARDI
 */
#include <AudioManager.hpp>
#include <cassert>
#include <Configuration.hpp>
#include <cstdlib>
#include <FileManager.hpp>
#include <Log.hpp>
#include <SDL2/SDL.h>
#ifdef CONFIGURATION_BUILD_FOR_MACOS
	#include <SDL2_mixer/SDL_mixer.h>
#else
	#include <SDL2/SDL_mixer.h>
#endif

namespace AudioManager
{
	/** How many musics are available. */
	#define MUSICS_COUNT (sizeof(_musics) / sizeof(Music))

	/** Associate a music file name to a playable handle. */
	typedef struct
	{
		const char *pointerStringFileName; //!< The file containing the music.
		Mix_Music *pointerMusicHandle; //!< The handle to use to play this music.
	} Music;

	/** Hold all sounds loaded into a chunk. */
	static Mix_Chunk *_pointerSounds[SOUND_IDS_COUNT];

	/** All available musics. */
	static Music _musics[] =
	{
		{
			CONFIGURATION_PATH_SOUNDS "/Akashic_Records_-_Epic_Action_Hero.ogg",
			NULL
		},
		{
			CONFIGURATION_PATH_SOUNDS "/Art_Music_-_Epic_Trailer.ogg",
			NULL
		},
		{
			CONFIGURATION_PATH_SOUNDS "/Blue_Giraffe_-_Action_Intense_Cinematic.ogg",
			NULL
		},
		{
			CONFIGURATION_PATH_SOUNDS "/Celestial_Aeon_Project_-_Epic.ogg",
			NULL
		},
		{
			CONFIGURATION_PATH_SOUNDS "/E._Erkut_-_Dark_Moment_-_Dark_Epic_Trailer.ogg",
			NULL
		},
		{
			CONFIGURATION_PATH_SOUNDS "/Matti_Paalanen_-_Emotion.ogg",
			NULL
		},
		{
			CONFIGURATION_PATH_SOUNDS "/Matti_Paalanen_-_Epic_Action.ogg",
			NULL
		},
		{
			CONFIGURATION_PATH_SOUNDS "/Soundbay_-_Epic_Future.ogg",
			NULL
		},
		{
			CONFIGURATION_PATH_SOUNDS "/Soundshrim_-_Epic_Adventure.ogg",
			NULL
		},
		{
			CONFIGURATION_PATH_SOUNDS "/Nico_Wohlleben_-_Storm.ogg",
			NULL
		}
	};

	/** Gather all sounds file names to load. */
	static const char *pointerStringsSoundFileNames[] =
	{
		CONFIGURATION_PATH_SOUNDS "/Ammunition_Taken.ogg",
		CONFIGURATION_PATH_SOUNDS "/Player_Fireshot.ogg",
		CONFIGURATION_PATH_SOUNDS "/Player_Fireshot_Mortar_Shell.ogg",
		CONFIGURATION_PATH_SOUNDS "/Player_Mortar_Shell_Reloading.ogg",
		CONFIGURATION_PATH_SOUNDS "/Player_Healed.ogg",
		CONFIGURATION_PATH_SOUNDS "/Player_Life_Increased.ogg",
		CONFIGURATION_PATH_SOUNDS "/Small_Enemy_Fireshot.ogg",
		CONFIGURATION_PATH_SOUNDS "/Medium_Enemy_Fireshot.ogg",
		CONFIGURATION_PATH_SOUNDS "/Big_Enemy_Fireshot.ogg",
		CONFIGURATION_PATH_SOUNDS "/Enemy_Bullet_Impact.ogg",
		CONFIGURATION_PATH_SOUNDS "/Small_Enemy_Explosion.ogg",
		CONFIGURATION_PATH_SOUNDS "/Medium_Enemy_Explosion.ogg",
		CONFIGURATION_PATH_SOUNDS "/Big_Enemy_Explosion.ogg",
		CONFIGURATION_PATH_SOUNDS "/Enemy_Spawner_Bullet_Impact.ogg",
		CONFIGURATION_PATH_SOUNDS "/Enemy_Spawner_Explosion.ogg",
		CONFIGURATION_PATH_SOUNDS "/Menu_Move.ogg",
		CONFIGURATION_PATH_SOUNDS "/Menu_Select.ogg",
		CONFIGURATION_PATH_SOUNDS "/Machine_Gun_Taken.ogg",
		CONFIGURATION_PATH_SOUNDS "/Bulletproof_Vest_Taken.ogg"
	};

	/** Make the thread wait until a new music must be played. */
	static SDL_cond *_pointerMusicThreadCondition;
	/** The boolean representing the condition state. */
	static int _isCurrentMusicFinished = 0;
	/** The mutex needed to use the thread condition. */
	//static SDL_mutex *_pointerMusicThreadMutex;
	/** The music thread that will pause between two musics and start the next one. */
	//static SDL_Thread *_pointerMusicThread;
	/** Tell the thread it must exit. */
	//static volatile int _isThreadTerminated = 0;

	/** Load a sound from a wave file.
	* @param pointerStringFileName The file to load.
	* @return A pointer on the chunk containing the sound.
	* @warning This function will stop the program if the sound can't be loaded.
	*/
	static Mix_Chunk *_loadFromWave(const char *pointerStringFileName)
	{
		// Try to load the file
		Mix_Chunk *pointerChunk = Mix_LoadWAV(pointerStringFileName);
		if (pointerChunk == NULL)
		{
			printf("Mix_loadWAV: %s\n", Mix_GetError());
			LOG_ERROR("Failed to load sound file '%s' (%s).", pointerStringFileName, Mix_GetError());
			exit(-1);
		}
		
		return pointerChunk;
	}

	/** Called when the previous music finished. Wake a thread that will start the music out of the callback scope, as requested by SDL Mixer documentation. */
	static void _wakeUpMusicThread()
	{
		// Wake up the thread
		// SDL_LockMutex(_pointerMusicThreadMutex);
		// _isCurrentMusicFinished = 1;
		// SDL_CondSignal(_pointerMusicThreadCondition);
		// SDL_UnlockMutex(_pointerMusicThreadMutex);

		playMusic();
	}

	/** Wait for _wakeUpMusicThread() signal, pause some time and play the next music.
	* @return Always 0.
	*/
	static int _playNextMusicThread(void *)
	{
		// while (!_isThreadTerminated)
		// {
		// 	// Wait for a wake up signal from the callback called when a music is finished
		// 	SDL_LockMutex(_pointerMusicThreadMutex);
		// 	while (!_isCurrentMusicFinished) SDL_CondWait(_pointerMusicThreadCondition, _pointerMusicThreadMutex);
		// 	SDL_UnlockMutex(_pointerMusicThreadMutex);
			
		// 	// Avoid waiting 2 seconds if the delay was not started
		// 	if (_isThreadTerminated) break;
			
		// 	// Pause 2 seconds to avoid directly starting a different music
		// 	SDL_Delay(2000);
			
		// 	playMusic();
		// 	_isCurrentMusicFinished = 0;
		// }
		
		// LOG_DEBUG("Music thread exited.");
		return 0;
	}

	int initialize()
	{
		// Open audio mixer
		// Chunk size has been randomly chosen due to extremely explicit documentation...
		if (Mix_OpenAudio(CONFIGURATION_AUDIO_SAMPLING_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) != 0) 
		{
			LOG_ERROR("Failed to open audio device (%s).", Mix_GetError());
			return -1;
		}
		
		// Set the amount of channels (i.e. how many sounds can be played simultaneously)
		Mix_AllocateChannels(CONFIGURATION_AUDIO_CHANNELS_COUNT); // This function can't fail, according to documentation
		
		// Load all sounds
		unsigned int i;
		for (i = 0; i < sizeof(pointerStringsSoundFileNames) / sizeof(pointerStringsSoundFileNames[0]); i++)
		{
			LOG_DEBUG("Loading sound file %s...", pointerStringsSoundFileNames[i]);
			_pointerSounds[i] = _loadFromWave(FileManager::getFilePath(pointerStringsSoundFileNames[i]));
		}
		LOG_DEBUG("Successfully loaded all sound files.");
		
		// Load all musics
		for (i = 0; i < MUSICS_COUNT; i++)
		{
			// Try to load the file
			_musics[i].pointerMusicHandle = Mix_LoadMUS(FileManager::getFilePath(_musics[i].pointerStringFileName));
			if (_musics[i].pointerMusicHandle == NULL)
			{
				LOG_ERROR("Failed to load music '%s' (%s).", _musics[i].pointerStringFileName, Mix_GetError());
				return -1;
			}
		}
		
		// Call a callback when playing a music has finished
		Mix_HookMusicFinished(_wakeUpMusicThread);
		
		// // Create the condition needed to synchronize the thread
		// _pointerMusicThreadCondition = SDL_CreateCond();
		// if (_pointerMusicThreadCondition == NULL)
		// {
		// 	LOG_ERROR("Failed to create the thread condition (%s).\n", SDL_GetError());
		// 	return -1;
		// }
		
		// // Create the mutex needed by the condition mechanism
		// _pointerMusicThreadMutex = SDL_CreateMutex();
		// if (_pointerMusicThreadMutex == NULL)
		// {
		// 	LOG_ERROR("Failed to create the condition mutex (%s).\n", SDL_GetError());
		// 	return -1;
		// }
		
		// // Create a thread that will pause between two musics and start the next one
		// _pointerMusicThread = SDL_CreateThread(_playNextMusicThread, "Music", NULL);
		// if (_pointerMusicThread == NULL)
		// {
		// 	LOG_ERROR("Failed to create the music thread (%s).\n", SDL_GetError());
		// 	return -1;
		// }
		
		return 0;
	}

	void uninitialize()
	{
		//Tell the thread to exit
		//_isThreadTerminated = 1;
		_wakeUpMusicThread();
		// Wait for the thread to terminate
		//SDL_WaitThread(_pointerMusicThread, NULL);
		
		// Remove thread synchronization objects
		//SDL_DestroyMutex(_pointerMusicThreadMutex);
		//SDL_DestroyCond(_pointerMusicThreadCondition);
		
		// Free all musics
		unsigned int i;
		for (i = 0; i < MUSICS_COUNT; i++) Mix_FreeMusic(_musics[i].pointerMusicHandle);
		
		// Free all sounds
		for (i = 0; i < SOUND_IDS_COUNT; i++) Mix_FreeChunk(_pointerSounds[i]);
		
		// Release audio mixer
		Mix_CloseAudio();
	}

	void playSound(SoundId id)
	{
		if (id < SOUND_IDS_COUNT)
		{
			// Try to play the sound on the first available channel
			if (Mix_PlayChannel(-1, _pointerSounds[id], 0) == -1) LOG_DEBUG("Failed to play sound ID %d (%s).", id, Mix_GetError());
		}
	}

	void playMusic()
	{
		// Select a random music
		int musicIndex = rand() % MUSICS_COUNT;
		
		// Try to play it
		if (Mix_PlayMusic(_musics[musicIndex].pointerMusicHandle, 1) != 0) LOG_ERROR("Failed to play music %s (%s).", _musics[musicIndex].pointerStringFileName, Mix_GetError());
		else LOG_DEBUG("Playing music '%s'.", _musics[musicIndex].pointerStringFileName);
	}

	void pauseMusic(bool isMusicPaused)
	{
		if (isMusicPaused) Mix_PauseMusic();
		else Mix_ResumeMusic();
	}

	void stopAllSounds()
	{
		Mix_HaltChannel(-1);
	}
}
