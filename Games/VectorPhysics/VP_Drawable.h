/*
 *  VP_Drawable.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/02/08.
 *  Copyright 2008 Trevor Powell. All rights reserved.
 *
 */

#ifndef VP_DRAWABLE_H
#define VP_DRAWABLE_H

#define MAX_SAMPLES (200)

class vpGame;

class vpDrawable : public vsEntity, public vsCollisionResponder
{
	vpGame			*m_game;
	vsColor			m_color;
	vsShader		*m_shader;
	int				m_drawMode;

	vsCollisionObject **	m_object;
	int				m_objectCount;

	vsVector3D		m_samplePos[MAX_SAMPLES];
	int				m_sampleCount;

//	int				m_index[MAX_SAMPLES];
//	int				m_indexCount;

	int				m_optimisedIndex[MAX_SAMPLES];
	int				m_optimisedIndexCount;

	float			m_error;

	float			m_colorPulse;

	bool			m_toDelete;
	bool			m_drawing;
	bool			m_hilighted;

	vsVector3D		CalculateOptimisedPositionForIndex(int index, int ignoringOptimisedIndex=-1);
	float			CalculateOptimisedError(int ignoringOptimisedIndex = -1);

	void			OptimiseOutOneIndex();
	void			RemoveOptimisedIndex(int id);
	void			Optimise_Rigid();
	void			Optimise_Rope();

	void			ConvertToPhysics();

	void			ConvertToPhysics_Rigid();
	void			ConvertToPhysics_Rope();


public:

	enum
	{
		DrawMode_Normal,
		DrawMode_Rope,
		DrawMode_Static,

		DRAWMODE_MAX
	};

	vpDrawable(vpGame *game, int drawMode);
	virtual ~vpDrawable();

	virtual void	Draw( vsRenderQueue *queue );
	virtual void	Update( float timeStep );

	void			SetHilighted(bool hilited) { m_hilighted = hilited; }
	bool			IsHilighted() { return m_hilighted; }

	void			SetToDelete() { m_toDelete = true; }
	bool			IsToDelete() { return m_toDelete; }

	virtual bool	CollisionCallback(const colEvent &) {return false;}
	virtual void	DestroyCallback();

	static	const vsColor &	GetColorForDrawMode( int mode );
};

#endif // VP_DRAWABLE_H

