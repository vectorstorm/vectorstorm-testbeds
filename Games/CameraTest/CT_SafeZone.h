/*
 *  CT_SafeZone.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/01/11.
 *  Copyright 2011 Trevor Powell. All rights reserved.
 *
 */

#ifndef CT_SAFE_ZONE_H
#define CT_SAFE_ZONE_H

class ctSafeZone : public vsEntity
{
public:
	
	ctSafeZone() {}
	
	virtual void Draw( vsRenderQueue *queue );
};

#endif // CT_SAFE_ZONE_H

