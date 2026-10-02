#include "choco.hpp"

class Filesystem{

    private:

	std::filesystem::path path = std::filesystem::current_path();
	std::vector<std::basic_string<char>> arguements;

	bool make_directory_init(){

	}

	bool list_init(){

	    for (auto &a : arguements){

		std::print("{}", a);

	    }

	}

	bool touch_init(){


	}

	bool change_directory_init(){

	}

    public:

	Filesystem(){}

	Filesystem(std::vector<std::basic_string<char>> args)
	
	    : arguements(args)

	{


	}

	void list(){

	    list_init();

	}
	void touch(){}
	void make_directory(){}
	void change_directory(){}

};

class Application{

    private:

	std::basic_string<char> profile_prompt = choco_profile::default_prompt();
	const bool is_running = true;

	void exit_program(){

	    std::print("exiting the program");
	    std::exit(EXIT_SUCCESS);

	    return void();
	}

	uint32_t dispatchCommand(std::basic_string_view<char> command){

	    using namespace choco_system;

	    uint32_t command_Id = COMMAND_NOT_FOUND_ID;

	    for (auto &current_command : choco_system::commands){

		if (current_command.commandName == command){

		    command_Id = static_cast<uint32_t> (current_command.commandId);

		}
	    }

	    return command_Id;
	}

	bool executeCommand(std::vector<std::basic_string<char>> tokens){

	    using namespace choco_system;
	    bool function_exited_safe = false;

	    Filesystem fsystem(tokens);

	    switch(dispatchCommand(tokens.front())){

		case COMMAND_NOT_FOUND_ID:

		    function_exited_safe = true;
		    return function_exited_safe; 

		case COMMAND_LS_ID:

		    fsystem.list();

		    function_exited_safe = true;
		    return function_exited_safe; 

		case COMMAND_CD_ID:

		    function_exited_safe = true;
		    return function_exited_safe; 

		case COMMAND_MKDIR_ID:

		    function_exited_safe = true;
		    return function_exited_safe; 

		case COMMAND_EXIT_ID:

		    exit_program();

		    function_exited_safe = true;
		    return function_exited_safe; 

		default:

		    throw std::runtime_error("Something's wrong with execute function!!!");

		    return function_exited_safe;

	    }

	    return function_exited_safe;

	}

	std::vector<std::basic_string<char>> split(std::basic_string<char> buffer){

	    std::vector<std::string> seperatedBuffer = {};
	    const char space = ' ';
	    std::string tempH;
	    uint32_t initialCount = 0;

	    for (char &currentCharacter : buffer){

		initialCount++;
		tempH.push_back(currentCharacter);

		if (currentCharacter == space){

		    tempH.pop_back();
		    if (tempH.empty()){
			continue;
		    }

		    seperatedBuffer.push_back(tempH);
		    tempH.clear();
		    continue;

		}

		if (initialCount == buffer.size()){

		    seperatedBuffer.push_back(tempH);
		    tempH.clear();
		    continue;

		}

	    }

	    return seperatedBuffer;

	}

	bool input_empty(const std::basic_string<char> str){

	    return str.empty();
	}

	void init(){

	}

	void loop(){

	    do{

		std::basic_string<char> user_input;

		std::print("{}", profile_prompt);
		std::getline(std::cin, user_input);

		if (input_empty(user_input)) continue;

		std::vector<std::basic_string<char>> tokens = split(user_input);

		if (!executeCommand(tokens)){

		    throw std::runtime_error("Something went wrong!!");

		}
		



	    }while(is_running);

	}

    public:

	void run(){

	    init();
	    loop();

	}

};

int main(){

    Application app;

    try{

	app.run();

    }catch(std::exception &error){

	std::cerr << error.what() << std::endl;
	
	return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;

}
