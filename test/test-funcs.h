#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Integer.h"

std::vector<grill::Integer::block_t> create_block_vector(const grill::Integer& n);

bool is_verbose();
