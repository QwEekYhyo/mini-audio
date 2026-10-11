# First connection

## Power On
`power on`


## Agent
No local input / ouput for pairing (doesn't seem to do the trick)
```
agent NoInputNoOutput
default-agent
```


## Make the device pairable
Usually already the case by default.

`pairable on`


## Make the device discoverable
`discoverable on`


## On pair
Services authorization request:

0000110d-0000-1000-8000-00805f9b34fb ->
A2DP (Advanced Audio Distribution Profile)

0000110e-0000-1000-8000-00805f9b34fb ->
AVRCP (Audio/Video Remote Control Profile)

0000111e-0000-1000-8000-00805f9b34fb ->
HFP (Hands-Free Profile)


# Player usage

## Go into player menu
`menu player`


## List players
`list`


## Select a default player
`select <player>`


## Pause the current track
`plause`

This will output:
```
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Status: paused
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Position: 0x00317627 (3241511)
```


## Play current track or an optionnal item
`play [item]`

This will output:
```
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Status: playing
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Position: 0x0000d87f (55423)
```


## Skip to next track
`next`

This will output:
```
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.Title: Est-ce que tu m'aimes ? - Pilule bleue
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.TrackNumber: 0x00000003 (3)
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.NumberOfTracks: 0x00000019 (25)
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.Duration: 0x00038a40 (232000)
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.Album: Mon coeur avait raison
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.Artist: GIMS
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Position: 0x00000130 (304)
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.Title: Reste
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.TrackNumber: 0x0000000a (10)
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.NumberOfTracks: 0x00000000 (0)
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.Duration: 0x00038860 (231520)
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.Album: Ceinture noire (Transcendance)
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Track.Artist: GIMS, Sting
[CHG] Player /org/bluez/hci0/dev_90_81_58_9D_84_E0/avrcp/player2 Position: 0x000000f3 (243)
```

So it seems like we get two tracks printed out? The first one might be the track that was playing right before that and then the next track.

# All Bluetoothctl commands

## Main menu
```
Menu main:
Available commands:
-------------------
advertise                                         Advertise Options Submenu
monitor                                           Advertisement Monitor Options Submenu
scan                                              Scan Options Submenu
gatt                                              Generic Attribute Submenu
le                                                LE Bearer Submenu
bredr                                             BREDR Bearer Submenu
admin                                             Admin Policy Submenu
player                                            Media Player Submenu
endpoint                                          Media Endpoint Submenu
transport                                         Media Transport Submenu
mgmt                                              Management Submenu
monitor                                           Advertisement Monitor Submenu
assistant                                         Media Assistant Submenu
hci                                               HCI Submenu
telephony                                         Telephony Submenu
list                                              List available controllers
show [ctrl]                                       Controller information
select <ctrl>                                     Select default controller
devices [Paired/Bonded/Trusted/Connected]         List available devices, with an optional property as the filter
system-alias <name>                               Set controller alias
reset-alias                                       Reset controller alias
power <on/off>                                    Set controller power
pairable <on/off>                                 Set controller pairable mode
discoverable <on/off>                             Set controller discoverable mode
discoverable-timeout [value]                      Set discoverable timeout
agent <on/off/auto/capability>                    Enable/disable agent with given capability
default-agent                                     Set agent as the default one
advertise <on/off/type>                           Enable/disable advertising with given type
set-alias <alias>                                 Set device alias
scan <on/off/bredr/le>                            Scan for devices
info [dev/set]                                    Device/Set information
pair [dev]                                        Pair with device
cancel-pairing [dev]                              Cancel pairing with device
trust [dev]                                       Trust device
untrust [dev]                                     Untrust device
block [dev]                                       Block device
unblock [dev]                                     Unblock device
remove <dev>                                      Remove device
connect <dev> [uuid]                              Connect a device and all its profiles or optionally connect a single profile only
disconnect [dev] [uuid]                           Disconnect a device or optionally disconnect a single profile only
wake [dev] [on/off]                               Get/Set wake support
bearer <dev> [last-seen/bredr/le]                 Get/Set preferred bearer
menu <name>                                       Select submenu
version                                           Display version
quit                                              Quit program
exit                                              Quit program
help                                              Display help about this program
export                                            Print environment variables
script <filename>                                 Run script
```

## Player menu
```
Menu player:
Available commands:
-------------------
list                                              List available players
show [player]                                     Player information
select <player>                                   Select default player
play [item]                                       Start playback
pause                                             Pause playback
stop                                              Stop playback
next                                              Jump to next item
previous                                          Jump to previous item
fast-forward                                      Fast forward playback
rewind                                            Rewind playback
equalizer <on/off>                                Enable/Disable equalizer
repeat <singletrack/alltrack/group/off>           Set repeat mode
shuffle <alltracks/group/off>                     Set shuffle mode
scan <alltracks/group/off>                        Set scan mode
change-folder <item>                              Change current folder
list-items [start] [end]                          List items of current folder
search <string>                                   Search items containing string
queue <item>                                      Add item to playlist queue
show-item <item>                                  Show item information
back                                              Return to main menu
version                                           Display version
quit                                              Quit program
exit                                              Quit program
help                                              Display help about this program
export                                            Print environment variables
script <filename>                                 Run script
```

# Notes

`cancel-pairing <dev>` failed, but `remove <dev>` seemed to have done the trick.

There's a discoverable timeout that can be configured.

I am not automatically reconnected to a paired device, so we could try to reconnect on power on, no pairing should be needed (unless the device has "forgotten" us).
For this the device has to be trusted apparently, so we can automatically trust devices after pairing maybe?

It could be interesting to select the player everytime we get told there's a new one.
