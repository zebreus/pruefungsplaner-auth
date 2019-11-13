#include "Server.hpp"

SecurityProviderHandler::SecurityProviderHandler(const string& peerAddress)
	: peerAddress(peerAddress)
{
	try {
		spdlog::info("{} connected", peerAddress);
        
	} catch (const TException& error) {
		if (dontCrash) {
			spdlog::error("{} failed in constructor; ThriftException ignored, because of --dont-crash: {}", peerAddress, error.what());
		} else {
			spdlog::critical("{} failed in constructor; ThriftException: {}", peerAddress, error.what());
			throw;
		}
	} catch (const exception& error) {
		if (dontCrash) {
			spdlog::error("{} failed in constructor; Unhandled exception ignored, because of --dont-crash: {}", peerAddress, error.what());
		} else {
			spdlog::critical("{} failed in constructor; Unhandled exception: {}", peerAddress, error.what());
			throw;
		}
	} catch (...) {
		if (dontCrash) {
			spdlog::error("{} failed in constructor; Unhandled error ignored, because of --dont-crash", peerAddress);
		} else {
			spdlog::critical("{} failed in constructor; Unhandled error", peerAddress);
			throw;
		}
	}
}

SecurityProviderHandler::~SecurityProviderHandler()
{
	spdlog::info("{} disconnected", peerAddress);
}

void SecurityProviderHandler::authorize(const std::string& username, const std::string& password) {
	spdlog::info("{} called authorize", peerAddress);
	try {
        
        spdlog::info("authorize");

	} catch (const TException& error) {
		spdlog::warn("{} failed in authorize; ThriftException: {}", peerAddress, error.what());
		throw;
	} catch (const exception& error) {
		if (dontCrash) {
			spdlog::error("{} failed in authorize; Unhandled exception ignored, because of --dont-crash: {}", peerAddress, error.what());
			InternalServerError terror;
			terror.message = "Internal server error, try again later.";
			throw terror;
		} else {
			spdlog::critical("{} failed in authorize; Unhandled exception: {}", peerAddress, error.what());
			throw;
		}
	} catch (...) {
		if (dontCrash) {
			spdlog::error("{} failed in authorize; Unhandled error ignored, because of --dont-crash", peerAddress);
			InternalServerError terror;
			terror.message = "Internal server error, try again later.";
			throw terror;
		} else {
			spdlog::critical("{} failed in authorize; Unhandled error", peerAddress);
			throw;
		}
	}
}

ClaimRequestStatus::type SecurityProviderHandler::requestClaim(const std::string& claim) {
	spdlog::info("{} called requestClaim", peerAddress);
	try {
        
        spdlog::info("requestClaim");
        return ClaimRequestStatus::GRANTED;

	} catch (const TException& error) {
		spdlog::warn("{} failed in requestClaim; ThriftException: {}", peerAddress, error.what());
		throw;
	} catch (const exception& error) {
		if (dontCrash) {
			spdlog::error("{} failed in requestClaim; Unhandled exception ignored, because of --dont-crash: {}", peerAddress, error.what());
			InternalServerError terror;
			terror.message = "Internal server error, try again later.";
			throw terror;
		} else {
			spdlog::critical("{} failed in requestClaim; Unhandled exception: {}", peerAddress, error.what());
			throw;
		}
	} catch (...) {
		if (dontCrash) {
			spdlog::error("{} failed in requestClaim; Unhandled error ignored, because of --dont-crash", peerAddress);
			InternalServerError terror;
			terror.message = "Internal server error, try again later.";
			throw terror;
		} else {
			spdlog::critical("{} failed in requestClaim; Unhandled error", peerAddress);
			throw;
		}
	}
}

void SecurityProviderHandler::getToken(std::string& _return) {
	spdlog::info("{} called getToken", peerAddress);
	try {
        
        spdlog::info("getToken");
        _return = "THIS_IS_A_TOKEN";

	} catch (const TException& error) {
		spdlog::warn("{} failed in getToken; ThriftException: {}", peerAddress, error.what());
		throw;
	} catch (const exception& error) {
		if (dontCrash) {
			spdlog::error("{} failed in getToken; Unhandled exception ignored, because of --dont-crash: {}", peerAddress, error.what());
			InternalServerError terror;
			terror.message = "Internal server error, try again later.";
			throw terror;
		} else {
			spdlog::critical("{} failed in getToken; Unhandled exception: {}", peerAddress, error.what());
			throw;
		}
	} catch (...) {
		if (dontCrash) {
			spdlog::error("{} failed in getToken; Unhandled error ignored, because of --dont-crash", peerAddress);
			InternalServerError terror;
			terror.message = "Internal server error, try again later.";
			throw terror;
		} else {
			spdlog::critical("{} failed in getToken; Unhandled error", peerAddress);
			throw;
		}
	}
}

SecurityProviderIf* SecurityProviderCloneFactory::getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
{
    std::shared_ptr<TSocket> sock = std::dynamic_pointer_cast<TSocket>(connInfo.transport);
	return new SecurityProviderHandler(sock->getPeerAddress());
}

void SecurityProviderCloneFactory::releaseHandler(SecurityProviderIf* handler)
{
	delete handler;
}

int main(int argc, char** argv)
{

	int serverPort;

	spdlog::level::level_enum logLevel = spdlog::level::info;
	spdlog::level::level_enum logfileLevel = spdlog::level::info;
	string logfileDirectory;

	//Parse options
	spdlog::debug("Parsing options");
	try {
		cxxopts::Options options(argv[0], "Security Provider server");
		options.add_options()
			("p,port", "The port on which the server listens", cxxopts::value<int>())
            ("dont-crash", "Catch all exceptions inside handlers", cxxopts::value<bool>(dontCrash))
            ("help", "Print help");
		options.add_options("Logging")
            ("d,debug", "Output information, errors and debug messages", cxxopts::value<bool>())
            ("i,info", "Output information and errors", cxxopts::value<bool>()->default_value("true"))
            ("e,error", "Output only errors", cxxopts::value<bool>())
            ("q,quiet", "Output nothing", cxxopts::value<bool>())
            ("log-directory", "Write logfiles into this directory", cxxopts::value<string>(logfileDirectory), "DIR")
            ("log-debug", "Output debug messages, information and errors to logfiles", cxxopts::value<bool>())
            ("log-info", "Output information and errors", cxxopts::value<bool>()->default_value("true"))
            ("log-error", "Output only errors", cxxopts::value<bool>())
            ("log-quiet", "Output nothing", cxxopts::value<bool>());
		auto result = options.parse(argc, argv);
		if (result.count("help") || result.arguments().size() == 0) {
			cout << options.help({ "", "Resource managment", "Logging" }) << std::endl;
			exit(EXIT_SUCCESS);
		}
		if (result.count("port")) {
			serverPort = result["port"].as<int>();
			if (serverPort > 65535 || serverPort < 1) {
				throw cxxopts::OptionException("Invalid port specified");
			}
		} else {
			throw cxxopts::OptionException("No port specified");
		}
		if (result.count("quiet") && result["quiet"].as<bool>()) {
			logLevel = spdlog::level::off;
		}
		if (result.count("error") && result["error"].as<bool>()) {
			logLevel = spdlog::level::err;
		}
		if (result.count("info") && result["info"].as<bool>()) {
			logLevel = spdlog::level::info;
		}
		if (result.count("debug") && result["debug"].as<bool>()) {
			logLevel = spdlog::level::trace;
		}
		if (result.count("log-quiet") && result["log-quiet"].as<bool>()) {
			logfileLevel = spdlog::level::off;
		}
		if (result.count("log-error") && result["log-error"].as<bool>()) {
			logfileLevel = spdlog::level::err;
		}
		if (result.count("log-info") && result["log-info"].as<bool>()) {
			logfileLevel = spdlog::level::info;
		}
		if (result.count("log-debug") && result["log-debug"].as<bool>()) {
			logfileLevel = spdlog::level::trace;
		}
	} catch (const cxxopts::OptionException& e) {
		spdlog::critical("Error parsing options: {}", e.what());
		exit(EXIT_FAILURE);
	}

	//Set logger
	spdlog::sink_ptr color = make_shared<spdlog::sinks::stdout_color_sink_mt>();
	color->set_level(logLevel);
	spdlog::init_thread_pool(8192, 1);
	auto logger = std::make_shared<spdlog::async_logger>("", color, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
	spdlog::drop("");
	spdlog::register_logger(logger);
	spdlog::set_default_logger(logger);
	logger->flush_on(spdlog::level::warn);
	spdlog::flush_every(std::chrono::minutes(1));
	logger->set_level(spdlog::level::trace);
	if (logfileDirectory != "") {
		try {
			filesystem::path baseLogfile(logfileDirectory);
			filesystem::create_directories(baseLogfile);
			baseLogfile.append("logfile");
			spdlog::sink_ptr dailyfile = std::make_shared<spdlog::sinks::daily_file_sink_mt>(baseLogfile.string(), 17, 54);
			dailyfile->set_level(logfileLevel);
			logger->sinks().push_back(dailyfile);
		} catch (const filesystem::filesystem_error& error) {
			spdlog::critical("Cannot access log directory");
			exit(EXIT_FAILURE);
		}
	}
	spdlog::debug("Logger initialized");

	//Initialize thrift server
	int port = serverPort;
	::std::shared_ptr<SecurityProviderProcessorFactory> processorFactory(std::make_shared<SecurityProviderProcessorFactory>(std::make_shared<SecurityProviderCloneFactory>()));
	::std::shared_ptr<TServerTransport> serverTransport(std::make_shared<TServerSocket>(port));
	::std::shared_ptr<TTransportFactory> transportFactory(std::make_shared<TBufferedTransportFactory>());
	::std::shared_ptr<TProtocolFactory> protocolFactory(std::make_shared<TBinaryProtocolFactory>());

	//Open thrift server
	spdlog::info("Starting server");
	TThreadedServer server(processorFactory, serverTransport, transportFactory, protocolFactory);
	server.serve();
	return 0;
}
