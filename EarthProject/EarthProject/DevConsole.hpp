class DevConsole
{
public:
  static void create();

private:
  static DevConsole *singletonInstance; // I am the singleton!

  DevConsole() { /* Ignore(). I am a singleton. I have no need for a public constructor.*/ };
  ~DevConsole() { /* Ignore(). I am a singleton.*/ };
};
