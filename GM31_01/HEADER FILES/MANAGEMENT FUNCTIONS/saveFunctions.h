#pragma once
#include <list>
#include <vector>
#include <array>

#include "main.h"
#include "renderer.h"
#include "functions.h"

//Cereal C++
#include <cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/unordered_map.hpp>

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