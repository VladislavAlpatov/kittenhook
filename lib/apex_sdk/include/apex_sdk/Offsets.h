//
// Created by vlad on 10/29/23.
//

#pragma once

#define OFFSET_ENTITYLIST 0x1D6B5D8 //cl_entitylist
#define OFFSET_LOCAL_ENT 0x2119848  //LocalPlayer might be moved to AVC_GameMovement
#define OFFSET_NAME_LIST 0xc264a60 //NameList
#define OFFSET_GLOBAL_VARS      0x16f4d20

#define OFFSET_LEVELNAME 0x16f51e0 //LevelName
#define OFFSET_CLIENTSTATE 0x16f5020 //ClientState
#define OFFSET_SIGNONSTATE 0x16f50cc //SignonState


#define OFFSET_TEAM 0x037c //m_iTeamNum
#define OFFSET_HEALTH 0x036c //m_iHealth
#define OFFSET_SHIELD 0x01a0 //m_shieldHealth
#define OFFSET_MAXSHIELD 0x01a4 //m_shieldHealthMax
#define OFFSET_ARMORTYPE 0x45c4 //m_armorType
#define OFFSET_NAME 0x04b9 //m_iName
#define OFFSET_SIGN_NAME 0x04b0 //m_iSignifierName
#define OFFSET_ABS_VELOCITY 0x0170 //m_vecAbsVelocity
#define OFFSET_VISIBLE_TIME 0x19C0 //CPlayer!lastVisibleTime

#define OFFSET_LIFE_STATE 0x06c8 //m_lifeState, >0 = dead
#define OFFSET_BLEED_OUT_STATE 0x2714 //m_bleedoutState, >0 = knocked

#define OFFSET_ORIGIN 0x017c //m_vecAbsOrigin - 3rd offset after the first int and vector
#define OFFSET_BONES 0x0dd0 + 0x48 //m_nForceBone + 0x48
#define OFFSET_STUDIOHDR 0x1020 //CBaseAnimating!m_pStudioHdr
#define OFFSET_AIMPUNCH 0x23f8 //m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle, first one
#define OFFSET_CAMERAPOS 0x1f00 //CPlayer!camera_origin
#define OFFSET_VIEWANGLES 0x2564 - 0x14 //m_ammoPoolCapacity - 0x14
#define OFFSET_BREATH_ANGLES OFFSET_VIEWANGLES - 0x10
#define OFFSET_OBSERVER_MODE 0x3454 //m_iObserverMode
#define OFFSET_OBSERVING_TARGET 0x3460 //m_hObserverTarget

#define OFFSET_MATRIX 0x11a350 //ViewMatrix
#define OFFSET_RENDER 0x741f0a8 //ViewRender displays ESp, heath dist names etc

#define OFFSET_WEAPON 0x1954 //m_latestPrimaryWeapons
#define OFFSET_BULLET_SPEED 0x19c8 + 0x04d4 //CWeaponX!m_flProjectileSpeed maybe its WeaponSettings.projectile_launch_speed now
#define OFFSET_BULLET_SCALE 0x19c8 + 0x04dc //CWeaponX!m_flProjectileScale maybe its WeaponSettings.projectile_gravity_scale now
#define OFFSET_AMMO 0x1574 //m_ammoInClip first offset
