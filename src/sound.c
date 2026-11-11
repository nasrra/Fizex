#include "vendors\fmod\x64\fmod.h"
#include "vendors\fmod\x64\fmod_common.h"
#include "vendors\fmod\x64\fmod_studio.h"
#include "vendors\fmod\x64\fmod_studio_common.h"

// FMOD_SYSTEM* fmod_system;
FMOD_STUDIO_SYSTEM* fmod_studio_system;

void fmod_error_check(FMOD_RESULT result){
    ASSERT(result == FMOD_OK, "result is not okay");
}

FMOD_STUDIO_BANK* fmod_master_bank;
FMOD_STUDIO_BANK* fmod_master_strings_bank;
FMOD_STUDIO_BANK* fmod_globals_bank;

FMOD_RESULT sound_one_shot_event_callback(
  FMOD_STUDIO_EVENT_CALLBACK_TYPE type,
  FMOD_STUDIO_EVENTINSTANCE *event,
  void *parameters
){
    if(type == FMOD_STUDIO_EVENT_CALLBACK_STOPPED){
        os_output_message("release sound!\n");
        return FMOD_Studio_EventInstance_Release(event);
    }
    return FMOD_OK;
}

#define SOUND_PLAY_ONE_SHOT_EVENT(_event_name)          \
    FMOD_STUDIO_EVENTDESCRIPTION* __sound001_event_desc;\
                                                        \
    fmod_error_check(                                   \
        FMOD_Studio_System_GetEvent(                    \
            fmod_studio_system,                         \
            _event_name,                                \
            &__sound001_event_desc                      \
        )                                               \
    );                                                  \
                                                        \
    FMOD_STUDIO_EVENTINSTANCE* __sound0002_event_inst;  \
                                                        \
    fmod_error_check(                                   \
        FMOD_Studio_EventDescription_CreateInstance(    \
            __sound001_event_desc,                      \
            &__sound0002_event_inst                     \
        )                                               \
    );                                                  \
    fmod_error_check(                                   \
        FMOD_Studio_EventInstance_SetCallback(          \
            __sound0002_event_inst,                     \
            sound_one_shot_event_callback,              \
            FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED    \
        )                                               \
    );                                                  \
                                                        \
    fmod_error_check(                                   \
        FMOD_Studio_EventInstance_Start(                \
            __sound0002_event_inst                      \
        )                                               \
    );

void sound_update(){
    FMOD_Studio_System_Update(fmod_studio_system);
    // fmod_error_check(FMOD_Studio_System_FlushCommands(fmod_studio_system));

    // FMOD_STUDIO_BUS* master_bus;
    
    // fmod_error_check( 
    //     FMOD_Studio_System_GetBus(
    //         fmod_studio_system,
    //         "bus:/",
    //         &master_bus
    //     )
    // );
    
    // f32 volume;
    // f32 final_volume;
    // fmod_error_check(
    //     FMOD_Studio_Bus_GetVolume(
    //         master_bus,
    //         &volume,
    //         &final_volume
    //     )
    // );
}

void sound_init(){
    i32 max_channels = 1024;

    fmod_error_check(FMOD_Studio_System_Create(&fmod_studio_system, FMOD_VERSION));
    fmod_error_check(FMOD_Studio_System_Initialize(fmod_studio_system, max_channels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
    
    fmod_error_check(
        FMOD_Studio_System_LoadBankFile(
            fmod_studio_system,
            "../assets/fmod/Desktop/Master.bank",
            FMOD_STUDIO_LOAD_BANK_NORMAL,
            &fmod_master_bank
        )
    );
    
    fmod_error_check(
        FMOD_Studio_System_LoadBankFile(
            fmod_studio_system,
            "../assets/fmod/Desktop/Master.strings.bank",
            FMOD_STUDIO_LOAD_BANK_NORMAL,
            &fmod_master_strings_bank
        )
    );

    fmod_error_check(
        FMOD_Studio_System_LoadBankFile(
            fmod_studio_system,
            "../assets/fmod/Desktop/Globals.bank",
            FMOD_STUDIO_LOAD_BANK_NORMAL,
            &fmod_globals_bank
        )
    );
    
    // sound_play_foo();
}

/*
    `remarks`
    `velocity` should be between 0 and 1.
*/
void sound_play_ball_hit(f32 velocity){
    SOUND_PLAY_ONE_SHOT_EVENT("event:/Sfx/ball_hit");
    FMOD_Studio_EventInstance_SetParameterByName(
        __sound0002_event_inst,
        "velocity",
        velocity,
        false
    );
}
void sound_play_cue_ball_hit(){SOUND_PLAY_ONE_SHOT_EVENT("event:/Sfx/cue_ball_hit");}