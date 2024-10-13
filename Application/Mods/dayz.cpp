#include "dayz.h"
#include <iostream>

void Game::start_mods()
{
	memory = Memory{ "DayZ_x64.exe" };

	base_address = memory.GetModuleAddress("DayZ_x64.exe");

	printf_s("[Memory] Base Address: 0x%llx\n", base_address);

	printf_s("[Memory] Getting World\n");
	world_address = memory.Read<uint64_t>(base_address + offsets::world);

	printf_s("[Memory] Getting Network Manager\n");
	net_manager = memory.Read<uint64_t>(base_address + offsets::net_manager);

	printf_s("[Memory] Getting Network Client\n");
	net_client = memory.Read<uint64_t>(base_address + offsets::net_manager + offsets::net_client);

	printf_s("[Memory] Getting Scoreboard\n");
	net_scoreboard = memory.Read<uint64_t>(net_client + offsets::net_scoreboard);

	printf_s("[Memory] World Address: 0x%llx\n", world_address);

	std::thread(&Game::update_entities, this).detach();
	std::thread(&Game::update_writes, this).detach();
}

void Game::update_entities()
{
	while (true)
	{
		uint64_t NearEntityList = memory.Read<uint64_t>(world_address + offsets::near_list);
		if (!NearEntityList) continue;
		uint32_t Size = memory.Read<uint64_t>(world_address + offsets::near_list + 0x8);

		std::vector<player> nearTemp{};

		for (int i = 0; i < Size; i++)
		{
			uint64_t Entity = memory.Read<uint64_t>(NearEntityList + (i * 0x8));
			if (!Entity) continue;

			uint32_t NetworkId = memory.Read<uint64_t>(Entity + offsets::player_network);

			player Player{};
			Player.EntPtr = Entity;
			Player.networkId = NetworkId;

			std::string type = GetEntityTypeName(Entity); 

			std::cout << NetworkId << std::endl;

			if (type == "dayzplayer")
			{
				// find name


				Player.type = 1;			// set 1 for player

				nearTemp.push_back(Player);
			}
			if (type == "dayzinfected")
			{
				Player.type = 2;			// set 2 for zombie

				nearTemp.push_back(Player);
			}
		}

		uint64_t FarEntityList = memory.Read<uint64_t>(world_address + offsets::far_list);
		if (!FarEntityList) continue;
		Size = memory.Read<uint64_t>(world_address + offsets::far_list + 0x8);

		std::vector<player> farTemp{};

		for (int i = 0; i < Size; i++)
		{
			uint64_t Entity = memory.Read<uint64_t>(FarEntityList + (i * 0x8));
			if (!Entity) continue;

			uint32_t NetworkId = memory.Read<uint64_t>(Entity + offsets::player_network);

			player Player{};
			Player.EntPtr = Entity;
			Player.networkId = NetworkId;

			std::string type = GetEntityTypeName(Entity);

			if (type == "dayzplayer")
			{
				// find name


				Player.type = 1;			// set 1 for player

				nearTemp.push_back(Player);
			}
			if (type == "dayzinfected")
			{
				Player.type = 2;			// set 2 for zombie

				nearTemp.push_back(Player);
			}
		}

		/*
		uint64_t ItemList = memory.Read<uint64_t>(world_address + offsets::item_list);
		if (!ItemList) continue;
		uint32_t Size = memory.Read<uint64_t>(world_address + offsets::item_list + 0x8);

		std::vector<player> itemTemp{};

		for (int i = 0; i < Size; i++)
		{


			

		}
		*/


		players.clear();
		players.insert(players.end(), nearTemp.begin(), nearTemp.end());
		players.insert(players.end(), farTemp.begin(), farTemp.end());

		Sleep(1000);
	}
}

void Game::update_players()
{
	while (true)
	{

	}
}

void Game::update_writes()
{

	float value;
	while (true)
	{
		
		if (config::bright_night)
		{
			std::cout << "Write 10.f" << std::endl;
			value = 10.f;
			memory.Write<float>(world_address + offsets::bright_night, 10.f);
		}
		else {
			value = -0.97f;
			memory.Write<float>(world_address + offsets::bright_night, value);
		}

		if (config::no_grass)
		{
			value = 0.f;
			memory.Write<float>(world_address + offsets::no_grass, value);
		}
		else {
			value = 1.f;
			memory.Write<float>(world_address + offsets::no_grass, value);
		}

		// TODO: Freecam

		Sleep(1000);
	}
}

std::string Game::GetEntityTypeName(uint64_t Entity)
{
	return memory.ReadArmaString(memory.Read<uint64_t>(memory.Read<uint64_t>(Entity + 0x180) + 0xA8)).c_str();
}
