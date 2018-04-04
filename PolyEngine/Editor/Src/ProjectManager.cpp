#include "PolyEditorPCH.hpp"

#include <LibraryLoader.hpp>
#include <AssetsPathConfig.hpp>

extern "C"
{
	using CreateGameFunc = Poly::IGame* (void);
}

static Poly::LibraryFunctionHandle<CreateGameFunc> LoadGame;

void ProjectManager::Create(const Poly::String& projectName, const Poly::String& projectPath, const Poly::String& enginePath)
{
	if (Opened)
		throw new ProjectManagerException("Can't create new project without closing previous one.");

	if (Running)
		throw new ProjectManagerException("Another operation is currently running.");

	Name = projectName;
	ProjectPath = projectPath;
	EnginePath = enginePath;

	Poly::StringBuilder builder;

	builder.Append("py Z:/Programming/C++/PolyEngine/PolyEngine/Scripts/ProjectTool.py -e ");
	builder.Append(EnginePath);
	builder.Append(" -c ");
	builder.Append(ProjectPath);
	builder.Append(" ");
	builder.Append(Name);

	Command = builder.GetString();
	CommandDesc = "Project creation";

	RunCommand(Command);
}

void ProjectManager::Open(const Poly::String& projectPath)
{
	if (Opened)
		throw new ProjectManagerException("Can't open project without closing previous one.");

	if (Running)
		throw new ProjectManagerException("Another operation is currently running.");

	// TODO (squares): get projectName from json
	ProjectConfig = std::make_unique<::ProjectConfig>(projectPath);
	ProjectPath = projectPath;

	// load game
	if (!LoadGame.FunctionValid())
	{
		// Load game library
		//LoadGame = Poly::LoadFunctionFromSharedLibrary<CreateGameFunc>(Poly::gAssetsPathConfig.GetGameLibPath().GetCStr(), "CreateGame");
		//LoadGame = Poly::LoadFunctionFromSharedLibrary<CreateGameFunc>("Z:/Desktop/Test1/Build/NewPolyPrject/Debug/NewPolyPrject.dll", "CreateGame");
		LoadGame = Poly::LoadFunctionFromSharedLibrary<CreateGameFunc>("Z:/Desktop/Debug/PolyJamGame.dll", "CreateGame");
		ASSERTE(LoadGame.FunctionValid(), "Library libGame load failed");
		Poly::gConsole.LogDebug("Library libGame loaded.");
	}

	std::unique_ptr<Poly::IGame> game = std::unique_ptr<Poly::IGame>(LoadGame());
	std::unique_ptr<Poly::IRenderingDevice> device = gApp->Ui.MainViewport->InitializeViewport();

	gApp->GameMgr.Init(std::move(game), std::move(device));

	Opened = true;
}

void ProjectManager::Update()
{
	if (!Opened)
		throw new ProjectManagerException("This operation requires any project opened.");

	if (Running)
		throw new ProjectManagerException("Another operation is currently running.");

	Poly::StringBuilder builder;

	builder.Append("py Z:/Programming/C++/PolyEngine/PolyEngine/Scripts/ProjectTool.py -e ");
	builder.Append(EnginePath);
	builder.Append(" -u ");
	builder.Append(ProjectPath);

	Command = builder.GetString();
	CommandDesc = "Project update";

	RunCommand(Command);

	Running = true;
}

void ProjectManager::Update(const Poly::String& enginePath)
{
	if (!Opened)
		throw new ProjectManagerException("This operation requires any project opened.");

	if (Running)
		throw new ProjectManagerException("Another operation is currently running.");

	EnginePath = enginePath;

	Poly::StringBuilder builder;

	builder.Append("py Z:/Programming/C++/PolyEngine/PolyEngine/Scripts/ProjectTool.py -e ");
	builder.Append(EnginePath);
	builder.Append(" -u ");
	builder.Append(ProjectPath);

	Command = builder.GetString();
	CommandDesc = "Project update";

	RunCommand(Command);

	Running = true;
}

void ProjectManager::Close()
{
	if (Running)
		throw new ProjectManagerException("Another operation is currently running.");

	Opened = false;

	Name = "";
	ProjectPath = "";
	EnginePath = "";
}

void ProjectManager::RunCommand(const Poly::String& cmd)
{
	if (Running)
		throw new ProjectManagerException("Another process is currently running within ProjectManager.");

	Poly::StringBuilder builder;

	// append some magic chars
	builder.Append(cmd);
	builder.Append(" 2>&1");

	// execute command and return output stream
#ifdef _WIN32
	Stream = _popen(builder.GetString().GetCStr(), "r");
#else
	Stream = popen(builder.GetString().GetCStr(), "r");
#endif

	Running = true;
	
	// setup timer for reading from stream
	Timer = std::make_unique<QTimer>(this);
	connect(Timer.get(), &QTimer::timeout, this, &ProjectManager::ReadStdout);
	Timer->start(0);

	// display some nice info
	*Ostream << "\n\n> ----------     " << CommandDesc << " started...     ----------\n";
	*Ostream << "> ----------     " << Command << "     ----------\n\n";
}

void ProjectManager::ReadStdout()
{
	if (Stream && !feof(Stream))
	{
		if (fgets(Buffer, MaxBuffer, Stream) != NULL)
			*Ostream << "> " << Buffer;
	}
	else
	{
#ifdef _WIN32
		_pclose(Stream);
#else
		pclose(Stream);
#endif
		Running = false;
		*Ostream << "\n> ----------     " << CommandDesc << " finished.     ----------\n\n";
		Command = "";
		CommandDesc = "";
	}
}
