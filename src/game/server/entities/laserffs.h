#ifndef GAME_SERVER_ENTITIES_LASERFFS_H
#define GAME_SERVER_ENTITIES_LASERFFS_H

class CFireBall : public CEntity
{
public:
	CLaserFFS(CGameWorld *pGameWorld, int Owner, vec2 Dir, vec2 Pos);

	virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);

private:
	vec2 m_Direction;
	int m_LifeSpan;
	int m_Owner;

	bool m_aFlagHits[2];
	bool m_aCharHits[MAX_CLIENTS];
};

#endif
