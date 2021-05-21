
# DayZ SA 0.62 Legacy Offline Mode
#### The following repository contains the mission files and scripts that allow you to play a local legitimate and fully legal version of DayZ Standalone patch 0.62 . The legacy patch of DayZ Standalone, the files are officially hosted and available to the public by Steam the publisher of the game (you must own the game).

## Table Of Contents:
- [Questions & Answers](#questions)
- [Install](#installation)
- [Features](#features)
- [Keybinds](#keybinds)
- [Buy me a coffee](#donations)
### Requirements:
* #### Own DayZ Standalone on Steam
* #### Access to a Steam Depot downloader tool (or Steam itself when they fix the depot downloader :D)
_______________
## Installation:
	Steam Depot Downloader: (two methods, method 2 does not require login information)
	
**Method #1**
* #### Step 1: Download the [DepotDownloader](https://github.com/SteamRE/DepotDownloader/releases/download/DepotDownloader_2.4.1/depotdownloader-2.4.1.zip) ([source code](https://github.com/SteamRE/DepotDownloader)) application, extract the files to where you desire (the game will also download at the same location). 
* #### Step 2: within the directory of your extracted files of DepotDownloader, navigate to "File" on the top left corner of the open window and click "Open Windows PowerShell"
![folder](https://i.imgur.com/igjEGum.png)

**Paste the following into the PowerShell prompt edit "username" and "password" to match your Steam login information and press ENTER, you may be prompted to input Steam Guard Authentication Code (you must use a Steam account that owns DayZ Standalone)**

	dotnet DepotDownloader.dll -app 221100 -depot 221101 -manifest 8062108413578003887 -username <username> -password <password>
	
* #### Step 3: The DepotDownloader will download the legacy patch within the same directory you have it installed under a newly created folder "depots". The download may take some time depending on your internet speeds.
	![PowerShell](https://i.imgur.com/EyiSF8v.png)

**Method #2**
* #### Step 1: Download the [SteamManifestPatcher](https://github.com/fifty-six/zig.SteamManifestPatcher/releases/download/v3/SteamDepotDownpatcher.exe) ([source code](https://github.com/fifty-six/zig.SteamManifestPatcher)) Make sure Steam is logged in and running! run the exe you just downloaded once, a window should popup and complete its task. The following software re-enabled the ability to download depots from Steam using the actual Steam interface.
	![Steam Depot Patcher](https://i.imgur.com/uGXXfMc.png)
* #### Step 2: open the link below in your browser, it will prompt you to "Open Steam Client Bootstrapper" which will open Steam client and show the Console window.
		steam://nav/console
* #### Step 3: Once the Steam console is open, paste the following into the text box and press ENTER this will start downloading the Legacy game files into your Steam content directory. Default full path: C:\Program Files (x86)\Steam\steamapps\content\app_221100\depot_221101
		download_depot 221100 221101 8062108413578003887
	![Steam Console](https://i.imgur.com/wOn8pEW.png)

#### So far, you have downloaded the required Legacy version of DayZ Standalone 0.62 Officially from Steam! Now to the next part, installing this Offline mod.

* #### Step 1: Download the following [repository](https://github.com/Da0ne/LegacyDayZOfflineMode/archive/refs/heads/main.zip) to your desired location. 
* #### Step 2: Extract the files in the root directory of the Legacy version of DayZ. If you followed Method #1, the files will be located within where you extracted "DepotDownloader" (drive:\depotdownloader-2.3.6\depots\221101\6650889) If you have followed Method #2, the files will be located in C:\Program Files (x86)\Steam\steamapps\content\app_221100\depot_221101 (modify the path according to your Steam installtion)
* #### Step 3: Once you have extracted the contents of this repository, it should look like the following screenshot below. 
	![DayZ 0.62 Directory](https://i.imgur.com/V2EEkk6.png)

* #### Step 4: Done! To start playing, make sure you have Steam running, start the Offline mode by simply running the batch file included: @PlayOfflineMode.bat the game should boot up, first bootup may take long due to the Central Loot Economy generating data. Depending on your drive speeds (recommended to use an SSD)

## Features:
* #### Able to play DayZ 0.62 locally with fully functional vanilla CLE (Central Loot Economy) setup (Infected AI, Animals, Wrecks, Events, Loot)
* #### Enabled extended Debug Tools developed and created fully by the official developers of the game
* #### Custom created player data save system (your play session is saved to file every 2 minutes and or on game exit)
* #### Able to enjoy truly my favorite version of DayZ Standalone Legacy Alpha 

## Keybinds
* #### [F1] Show/Hide help screen
* #### [LWin] + [TAB] Toggle Scene Editor (houses many features such as spawning items, teleporting, weather editing)
* #### [Right Shift]  Spawns item from clipboard (full list of classnames [here](https://pastebin.com/ejFRcarR), can also be found in the Scene Editor menu)
* #### [Insert] Teleport player to crosshairs
* #### [Backspace] Toggle freecamera (WASD, QZ, Left shift, mouse scroll for speed)
* #### [Page Down] Toggle Godmode (no damage)

## Questions
**Q:** **Is this gonna get me in trouble / banned?**

**A:** No. This method is 100% legitimate, it involves no piracy and or cheating in any shape or form. You are downloading a set of organized **mission files** that will start the game in a mode that was specially **created by the developer**. The same steps and methods were actually used in [Beta 0.63 Experimental Offline mode](https://www.youtube.com/watch?v=Y_o_i8b66NY)

**Q:** **Where does the player data save?**

**A:** Your player data is saved in a folder called **"Logs\db\alive"** which can be located in the root directory of the game files. Upon death / respawn, the older data files of your player will be moved to the directory called **"dead"** if you wish to restore your progress, simply exit the game after dying, move the data within the folder **"dead"** to **"alive"**

**Q:** **How can i reset the database of the loot?**

**A:** You can simple replace the folder **storage_-1** located in the root directory of the game with the original files from this repository, this will regenerate loot. Keep in mind, the CLE will generate loot as long as you play, the world as active and fully dynamic, you don't have to regenerate loot manually by editing files.

## Screenshots:
*Ingame screenshots credits to* [@Derleth](https://twitter.com/Derlethe)

![enter image description here](https://pbs.twimg.com/media/E1kuatWX0AA_Sao?format=jpg&name=4096x4096)

![enter image description here](https://pbs.twimg.com/media/E1kc8PTWYAI9yFr?format=jpg&name=4096x4096)

![enter image description here](https://cdn.discordapp.com/attachments/840956819954204685/844616137664233502/unknown.png)


![enter image description here](https://media.discordapp.net/attachments/840956819954204685/844616152197496852/unknown.png?width=1215&height=702)

![enter image description here](https://media.discordapp.net/attachments/840956819954204685/844616170941448282/unknown.png?width=1219&height=702)

## Why...?
**For those players who wanted to try out Legacy DayZ, and the nostalgic feelz, also so DOJ can stop annoying everyone about volumetric clouds. FeelzGoodMan**

## Donations
#### Appreciated, never required. [Support me](https://www.dayzvpp.com/donate) with what i do :) Thank you!
