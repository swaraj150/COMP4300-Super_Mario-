#pragma once
#include "Components.h"
#include <string>
#include <tuple>


typedef std::tuple<CTransform, CBoundingBox, CAnimation, CLifeSpan,CInput, CGravity, CState> ComponentsTuple;
enum eLevelEntities {
	PLAYER=0,
	BRICK,
	CLOUDBIG,
	BUSHBIG,
	BUSHSMALL,
	FLAGPOLE,
	PIPESMALL,
	BLOCK,
	BUSH,
	COIN,
	CLOUDSMALL,
	BULLET,
	SMALLBUSH,
	PIPE,
	BUSHER = 16,
	GOOMBA,
	BCLOUD = 18,
	GROUND=20,
	QUESTION=22,
	LAST
};

enum eLevelEntitiesType {
	DYNAMIC,
	TILE,
	DEC,
};
class Entity {
	friend class EntityManager;
	eLevelEntitiesType		m_id;
	eLevelEntities			m_tag;
	bool					m_Active = true;
	ComponentsTuple			m_components = std::make_tuple(CTransform(),
														   CBoundingBox(),
														   CAnimation(),
														   CLifeSpan(5),
														   CInput(),
														   CGravity(),
														   CState());
	Entity(eLevelEntitiesType& id, eLevelEntities& entityTag);
	Entity();

public:
	void destroy();
	eLevelEntitiesType id() const;
	bool isActive() const;
	eLevelEntities tag() const;

	template<typename T>
	bool hasComponent() {
		return getComponent<T>().has;
	}

	template<typename T>
	T& getComponent() {
		return std::get<T>(m_components);
	}
	template<typename T, typename ...TArgs>
	T& addComponent(TArgs &&...mArgs) {
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);


		/*	T(std::forward<TArgs>(mArgs)...) :
				This is a variadic parameter pack expansion using the ellipsis ....It means that the arguments in mArgs will be expanded and passed as arguments to the constructor of type T.

			std::forward<TArgs>(mArgs) :
				This part of the expression uses std::forward for perfect forwarding.It ensures that the correct value category(lvalue or rvalue) and const - qualification of each argument in mArgs is preserved when forwarding it to the constructor of type T.The TArgs template parameter represents a parameter pack of types.

			T(...) :
				The ellipsis ... is the pack expansion syntax.It expands the arguments obtained from std::forward<TArgs>(mArgs) and passes them to the constructor of type T.This is how the arguments are effectively forwarded to the constructor.

			component = T(std::forward<TArgs>(mArgs)...) :
				This assigns the result of the constructor(constructed from the forwarded arguments) to the component object.The type T should have a suitable constructor that matches the types and number of arguments being passed*/

		//This line constructs a new instance of the component T using perfect forwarding (std::forward) with the provided arguments (mArgs). The existing component is then assigned the newly constructed one. This assumes that the T type has a suitable constructor that accepts the forwarded arguments.

		/*	Lvalue: An lvalue represents an object with a persistent identity that you can reference.Examples include variables and non - temporary objects.
			Rvalue : An rvalue represents a temporary or movable value, often the result of an expression or a literal.Examples include literals, temporary objects, and the result of expressions like a + b.

			T&& arg (Rvalue Reference):
				Represents an rvalue reference.
				Can bind to temporary objects (rvalues).
				Typically used in move semantics and perfect forwarding.
				Useful for implementing functions that efficiently transfer ownership or manipulate temporary objects.

			T& arg (Lvalue Reference):

				Represents an lvalue reference.
				Can only bind to non-temporary objects (lvalues).
				Used for referencing existing objects.
				Used when you want to operate on or modify the value of an existing object.
		
		
			Use T&& when you want to create a function that accepts both lvalues and rvalues,since, T&& can bind to both lvalues and rvalues. 
			often for the purpose of implementing perfect forwarding or move semantics.
		
		
		*/
		component.has = true;
		return component;
	}
	template<typename T>
	void removeComponent() {
		getComponent<T>() = T();
	}
};
