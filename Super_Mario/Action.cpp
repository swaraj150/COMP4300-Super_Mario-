#include "Action.h"
Action::Action() {};

Action::Action(eActionNames names, const eActionTypes& types) :m_name(names), m_type(types) {};

eActionNames Action::name() const { return m_name; }

eActionTypes Action::type() const { return m_type; }