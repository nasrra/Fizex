#include "vendors\fmod\x64\fmod.h"
#include "vendors\fmod\x64\fmod_common.h"
#include "vendors\fmod\x64\fmod_studio.h"
#include "vendors\fmod\x64\fmod_studio_common.h"

// FMOD_SYSTEM* fmod_system;
FMOD_STUDIO_SYSTEM* fmod_studio_system;

void fmod_error_check(FMOD_RESULT result){
    ASSERT(result == FMOD_OK, "fmod result is not okay");
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

bool sound_play_one_shot_event(char* event_name, FMOD_STUDIO_EVENTINSTANCE** out_event_inst){
    
    FMOD_RESULT result;
    
    FMOD_STUDIO_EVENTDESCRIPTION* event_desc;
      
    result = FMOD_Studio_System_GetEvent(                  
        fmod_studio_system,                       
        event_name,
        &event_desc                    
    );
    
    if(result != FMOD_OK){
        ASSERT(false, "fmod result is not okay");
        return false;
    }
                                                
    FMOD_STUDIO_EVENTINSTANCE* event_inst;
                                                
    result = FMOD_Studio_EventDescription_CreateInstance(  
        event_desc,                    
        &event_inst                   
    );
                                         
    if(result != FMOD_OK){
        ASSERT(false, "fmod result is not okay");
        return false;
    }
    
    result = FMOD_Studio_EventInstance_SetCallback(        
        event_inst,                   
        sound_one_shot_event_callback,            
        FMOD_STUDIO_EVENT_CALLBACK_STOPPED  
    );
    
    if(result != FMOD_OK){
        ASSERT(false, "fmod result is not okay");
        return false;
    }
                                                
    result = FMOD_Studio_EventInstance_Start(              
        event_inst                    
    );                                             

    if(result != FMOD_OK){
        ASSERT(false, "fmod result is not okay");
        return false;
    }
    
    // fmod_error_check(FMOD_Studio_System_FlushCommands(fmod_studio_system));

    FMOD_STUDIO_BUS* master_bus;
    
    fmod_error_check( 
        FMOD_Studio_System_GetBus(
            fmod_studio_system,
            "bus:/",
            &master_bus
        )
    );

    fmod_error_check(
        FMOD_Studio_Bus_SetVolume(
            master_bus,
            0.8f
        )
    );
    
    *out_event_inst = event_inst;
    return true;
}

void sound_update(){
    FMOD_Studio_System_Update(fmod_studio_system);
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
bool sound_play_ball_hit_ball(f32 velocity){
    FMOD_STUDIO_EVENTINSTANCE* event_inst;
    if(!sound_play_one_shot_event("event:/Sfx/ball_hit_ball", &event_inst)){
        return false;
    }
    
    FMOD_Studio_EventInstance_SetParameterByName(
        event_inst,
        "velocity",
        velocity,
        false
    );

    // os_output_message("asd\n");

    return true;
}

/*
    `remarks`
    `velocity` should be between 0 and 1.
*/
bool sound_play_ball_hit_wall(f32 velocity){
    FMOD_STUDIO_EVENTINSTANCE* event_inst;
    if(!sound_play_one_shot_event("event:/Sfx/ball_hit_wall", &event_inst)){
        return false;
    }
    
    FMOD_Studio_EventInstance_SetParameterByName(
        event_inst,
        "velocity",
        velocity,
        false
    );

    return true;
}

/*
    `remarks`
    `velocity` should be between 0 and 1.
*/
bool sound_play_cue_ball_hit(f32 velocity){

    FMOD_STUDIO_EVENTINSTANCE* event_inst;
    if(!sound_play_one_shot_event("event:/Sfx/cue_ball_hit", &event_inst)){
        return false;
    }
    
    FMOD_Studio_EventInstance_SetParameterByName(
        event_inst,
        "velocity",
        velocity,
        false
    );
    
    return true;
}

bool sound_play_ball_hit_pocket_interior_wall(f32 velocity){
    FMOD_STUDIO_EVENTINSTANCE* event_inst;
    
    if(!sound_play_one_shot_event("event:/Sfx/ball_hit_pocket_interior_wall", &event_inst)){
        return false;
    }
    
    FMOD_Studio_EventInstance_SetParameterByName(
        event_inst,
        "velocity",
        velocity,
        false
    );
    
    return true;
}

bool soud_play_number_ball_pocketed(){
    FMOD_STUDIO_EVENTINSTANCE* event_inst;
    if(!sound_play_one_shot_event("event:/Sfx/number_ball_pocketed", &event_inst)){
        return false;
    }
    
    return true;
}