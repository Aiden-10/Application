#pragma once
#include <thread>
#include <vector>

#include "_memory.h"
#include "../include/Aiden_Log/a_log.h"
#include "../UIs/menuconfig.hpp"
#include "offsets.h"
#include "vector.h"

struct player {
	uint64_t EntPtr;
	std::string clean;
	int type;				// 1 for player, 2 for zombie
	uint32_t networkId;
};

struct item {
	uint64_t EntPtr;
	std::string clean;
	uint32_t networkId;
	Vector3 position;
};

struct net {
	std::string name;
	uint32_t networkId;
};

class Game
{
public:
	Game(std::string_view processName) : memory(processName), base_address(0), world_address(0),
		camera_address(0), local_player(0), net_manager(0), net_client(0), net_scoreboard(0)
	{
		
	}

	void start_mods();

	// void restart_mods();

private:
	void update_entities();
	void update_players();
	void update_writes();

	Memory memory;
	
	std::string GetEntityTypeName(uint64_t Entity);
private:
	uint64_t base_address;
	uint64_t world_address;
	uint64_t camera_address;
	uint64_t local_player;
	uint64_t net_manager;
	uint64_t net_client;
	uint64_t net_scoreboard;

public:
	std::vector<player> players;
};