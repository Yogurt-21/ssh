
#pragma once 

#include <iostream>
#include <vector>
#include <exception>
#include <print>
#include "ansi.h"
#include <format>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <filesystem>

namespace choco_profile{

    struct ProfileInfo{

	CoAnsiSpatialMode sMode;
	CoEscapeCodesColors color;
	std::string_view prompt;

    };

    std::basic_string<char> default_prompt(){

	ProfileInfo pInfo = {};

	pInfo.sMode = CO_FOREGROUND;
	pInfo.color = CO_ECC_YELLOW;
	pInfo.prompt = "->";

	return std::format(

		"\x1B[{};5;{}m{}",
		std::to_string(pInfo.sMode),
		std::to_string(pInfo.color),
		pInfo.prompt

	);

    }

}

namespace choco_system{

    enum CommandId{

	COMMAND_NOT_FOUND_ID = 0,
	COMMAND_LS_ID = 1,
	COMMAND_CD_ID = 2,
	COMMAND_MKDIR_ID = 3,
	COMMAND_EXIT_ID = 4

    };

    struct CommandInfo{

	std::basic_string<char> commandName;
	CommandId commandId;

    };

    constexpr CommandInfo COMMAND_LS{

	"ls", COMMAND_LS_ID

    };

    constexpr CommandInfo COMMAND_CD{

	"cd", COMMAND_CD_ID

    };

    constexpr CommandInfo COMMAND_MKDIR{

	"mkdir", COMMAND_MKDIR_ID

    };

    constexpr CommandInfo COMMAND_EXIT{

	"exit", COMMAND_EXIT_ID

    };

    std::vector<CommandInfo> commands{

	COMMAND_LS,
	COMMAND_CD,
	COMMAND_MKDIR,
	COMMAND_EXIT

    };

}


