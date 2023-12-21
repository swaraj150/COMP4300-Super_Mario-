#pragma once
 
enum eActionTypes {
	END=0,
	START
};

enum eActionNames {
	TOGGLE_TEXTURE=0,
	TOGGLE_COLLISION,
	TOGGLE_GRID,
	PAUSE,
	QUIT,
	UP,
	RIGHT,
	LEFT,
	FIRE
};

class Action {
	eActionTypes m_type;
	eActionNames m_name;
public:
	Action();
	Action(eActionNames names, const eActionTypes &types);

	eActionNames name() const;
	eActionTypes type() const;
};