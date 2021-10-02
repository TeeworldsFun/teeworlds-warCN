/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "soldierdec.h"
#include "flag.h"
#include "ffslaser.h"


CSoldierDec::CSoldierDec(CGameWorld *pGameWorld, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PROJECTILE)
{
	m_Owner = Owner;
	if(GameServer()->GetPlayerChar(m_Owner))
        m_Pos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

	GameWorld()->InsertEntity(this);
}

void CSoldierDec::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CSoldierDec::Fire()
{
    if(m_ReloadTimer)
        return;

    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        CCharacter* pHit = GameServer()->GetPlayerChar(i);

        if(!pHit)
            continue;

        if(pHit->GetPlayer()->GetTeam() == GameServer()->m_apPlayers[m_Owner]->GetTeam())
            continue;

        if(distance(pHit->m_Pos, m_Pos) > 1000)
            continue;

        if(GameServer()->Collision()->IntersectLine(m_Pos, pHit->m_Pos, NULL, NULL))
            continue;

        vec2 Dir = normalize(pHit->m_Pos - m_Pos);

        new CLaserFFS(&GameServer()->m_World, m_Owner, Dir, m_Pos);
        m_ReloadTimer = Server()->TickSpeed() * 2;

        return;
    }

    for(int i = 0; i < 2; i ++)
    {
        CFlag* pFlag = GameServer()->m_apFlags[i];

        if(pFlag->m_Team == GameServer()->m_apPlayers[m_Owner]->GetTeam())
            continue;

        if(distance(m_Pos, pFlag->m_Pos) > 1000)
            continue;

        if(GameServer()->Collision()->IntersectLine(m_Pos, pFlag->m_Pos, NULL, NULL))
            continue;

        vec2 Dir = normalize(pFlag->m_Pos - m_Pos);

        new CLaserFFS(&GameServer()->m_World, m_Owner, Dir, m_Pos);
        m_ReloadTimer = Server()->TickSpeed() * 2;

        return;
    }
}


void CSoldierDec::Tick()
{
    if(!GameServer()->GetPlayerChar(m_Owner))
    {
        Reset();
        return;
    }

    vec2 CharPos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

    m_Pos = CharPos + vec2(0, 20);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 20);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 20);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 20);
}

void CSoldierDec::Snap(int SnappingClient)
{
	CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_Type = POWERUP_WEAPON;
	pObj->m_Subtype = WEAPON_RIFLE;
}
