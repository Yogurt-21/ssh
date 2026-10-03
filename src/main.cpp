#include "choco.hpp"

//temporary
std::filesystem::path user_path = "/home/jaz";
std::filesystem::path current_path = user_path;

class Filesystem{

    private:
	
	std::vector<std::basic_string<char>> arguements;
	bool exit_success = false;
	std::error_code ec;

	bool fileExist(std::basic_string<char> path){

	    return std::filesystem::exists(path);
	}

	void fsystem_logs(std::basic_string<char> logs){

	    std::println("({})", logs);

	}

	bool make_directory_init(){

	    arguements.erase(arguements.begin());

	    for (std::basic_string<char> arguement : arguements){

		if (fileExist(arguement)){

		    const char yes_option = 'y';
		    const char no_option = 'n';
		    char user_input;

		    fsystem_logs("The file exist!");
		    fsystem_logs("Do you still want to overwrite it?(y/n): ");

		    do{

			user_input = std::getchar();

		    }while(user_input == yes_option || user_input == no_option);

		    if (user_input == no_option){

			if (VERBOSE){

			    fsystem_logs("not making directory for this path");
			    fsystem_logs(arguement);

			}

			continue;
		    }

		}else if (!fileExist(arguement)){

		    if (!std::filesystem::create_directory(arguement, ec)){

			fsystem_logs("failed in create directory function for this path!");
			fsystem_logs(arguement);

		    }

		}


	    }

	    exit_success = true;
	    return exit_success;

	}

	bool list_init(){

	    const char hidden_file = '.';
	    arguements.erase(arguements.begin());

	    if (!arguements.empty()){

		for (std::basic_string<char> arguement : arguements){

		    //there's a problem with these.

		    if (arguement == " "){

			for (
			    std::filesystem::path current_iterator : 
			    std::filesystem::directory_iterator(current_path)
			){

			    std::basic_string<char> str_iterator = 
				static_cast<std::basic_string<char>> (current_iterator.filename());

			    if (str_iterator.front() == hidden_file){

				continue;

			    }

			    std::println("{}", str_iterator);

			}

		    }

		    if (!fileExist(arguement)){

			fsystem_logs("The path doesn't exist!");
			break;

		    }

		    for (
			std::filesystem::path current_iterator : 
			std::filesystem::directory_iterator(arguement)
		    ){

			std::basic_string<char> str_iterator = 
			    static_cast<std::basic_string<char>> (current_iterator.filename());

			if (str_iterator.front() == hidden_file){

			    continue;

			}

			std::println("{}", str_iterator);

		    }

		}

		exit_success = true;

		return exit_success;

	    }else if (arguements.empty()){

		for (
		    std::filesystem::path current_iterator : 
		    std::filesystem::directory_iterator(current_path)
		){

		    std::basic_string<char> str_iterator = 
			static_cast<std::basic_string<char>> (current_iterator.filename());

		    if (str_iterator.front() == hidden_file){

			continue;

		    }

		    std::println("{}", str_iterator);

		}

		exit_success = true;

		return  exit_success;

	    }

	    return exit_success;

	}

	bool touch_init(){


	}

	bool change_directory_init(){

	    arguements.erase(arguements.begin());

	    for (std::basic_string<char> arguement : arguements){

		current_path /= static_cast<std::filesystem::path> (arguement);

		if (!fileExist(current_path)){

		    break;
		}

		std::filesystem::current_path(current_path);

	    }

	    exit_success = true;
	    return exit_success;
	}

    public:

	Filesystem(std::vector<std::basic_string<char>> &args)
	
	    : arguements(args)

	{


	}

	void list(){

	    if (!list_init()){

		throw std::runtime_error("list command failed!");

	    }

	    if (VERBOSE){

		fsystem_logs("list command success!!");

	    }

	}
	void touch(){}
	void make_directory(){

	    if (!make_directory_init()){

		throw std::runtime_error("make directory command failed!");

	    }

	    if (VERBOSE){

		fsystem_logs("make directory command success!!");

	    }

	}
	void change_directory(){

	    if (!change_directory_init()){

		throw std::runtime_error("change directory command failed!");

	    }

	}

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

		    fsystem.change_directory();

		    function_exited_safe = true;
		    return function_exited_safe; 

		case COMMAND_MKDIR_ID:

		    fsystem.make_directory();

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
