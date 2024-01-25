#pragma once
#include <list>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <thread>
#include <fstream>

#include "main.h"
#include "renderer.h"
#include "functions.h"
#include "input.h"

#include "modelReader.h"
#include "textureReader.h"
#include "soundReader.h"

//Cereal C++
#include <cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/unordered_map.hpp>

//ImGUI
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_win32.h"
#include "../imGUI/imgui_impl_dx11.h"
#include "../imGUI/implot.h"

class D3DXBOOL3
{
public:
	bool x;
	bool y;
	bool z;

public:

	D3DXBOOL3() { x = false; y = false; z = false; }
	D3DXBOOL3(bool a, bool b, bool c) :x(a), y(b), z(c) {}

	D3DXBOOL3 operator=(const D3DXBOOL3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	bool operator==(const bool& other)
	{
		if (x == other) { return true; }
		if (y == other) { return true; }
		if (z == other) { return true; }

		return false;
	}

};

template<class Archive>
void serialize(Archive & archive, D3DXVECTOR2 & vector)
{
	archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y));
}
template<class Archive>
void serialize(Archive & archive, D3DXVECTOR3 & vector)
{
	archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y), cereal::make_nvp("z", vector.z));
}
template<class Archive>
void serialize(Archive & archive, D3DXCOLOR & vector)
{
	archive(cereal::make_nvp("r", vector.r), cereal::make_nvp("g", vector.g), cereal::make_nvp("b", vector.b), cereal::make_nvp("a", vector.a));
}

template<class Archive>
void serialize(Archive & archive, ImPlotPoint & vector)
{
	archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y));
}

template<class Archive>
void serialize(Archive & archive, D3DXBOOL3 & vector)
{
	archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y), cereal::make_nvp("z", vector.z));
}

#include "gameobject.h"
#include "component.h"
#include "material.h"
#include "baseobjects.h"
#include "prefab.h"