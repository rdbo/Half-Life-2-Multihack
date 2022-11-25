<h1> HL2Ware.net - Half-Life 2 Cheat </h1>
HL2Ware.net is updated fork of <a href="https://github.com/rdbo/Half-Life-2-Multihack">rdbo/Half-Life-2-Multihack</a>


<h3>Menu screenshot: </h3>

![image](https://user-images.githubusercontent.com/65111609/204060232-d0ebb64c-3c96-44d7-9d2a-165fac0f3511.png)


<h2>Functions</h2>

```
Inf HP
Inf Armor
Inf Aux Power
Inf Ammo
Auto BunnyHop
Onetap kills
TriggerBot
Break MaxSpeed
Instant Duck
No View Punch
Wireframe (Wallhack)
Console Logs
```

<h2>How to use</h2>

```
U will need: Visual Studio (2019 or 2022 recommended), DirectX SDK, Half-life 2(Optional but you must somehow test this), Injector(i recommend Process hacker for injecting)

1. Open .sln file
2. Add DX SDK to VC++ Directories
3. go to build settings and change Debug to Release, make sure that you are building to x86 platform not x64
4. Compile.
5. open Half-Life 2
6. Inject HL2ware.net-master\Release\HalfLife2_Multihack.dll to hl2.exe
7. Done! 
```


<h1>How to install DirectX sdk</h1>

```
Download and install https://www.microsoft.com/en-us/download/details.aspx?id=6812
Open the solution on Visual Studio and open the project Properties
Go to VC++ Directories -> Include Directories. Click on 'Edit' and select the Include folder located on your DirectX SDK installation path. 
It is generally this one: <br/><i>%programfiles(x86)%\Microsoft DirectX SDK (June 2010)\Include\</i> or <i>%DXSDK_DIR%\Include\
Now go to VC++ Directories -> Library Directories. Click on 'Edit' and select the library folder located on your DirectX SDK installation path. 
It is generally this one - choose x86 for 32bit and x64 for 64bit: <br/><i>%programfiles(x86)%\Microsoft DirectX SDK (June 2010)\Lib\</i> or <i>%DXSDK_DIR%\Lib\
Done!
```
