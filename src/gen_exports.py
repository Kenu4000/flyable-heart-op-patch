import re

text = r"""
10001110 エクスポート 16  PalAttachWorkProcess
10001160 エクスポート 15  PalAttachProcess
10001180 エクスポート 71  PalGetCore
100011A0 エクスポート 137 PalSetAppType
100011C0 エクスポート 1   Pal3DIs
100011E0 エクスポート 33  PalDebugIs
100011F0 エクスポート 72  PalGetFilePath
10001200 エクスポート 70  PalGetCmdLine
10001210 エクスポート 74  PalGetWindowHandle
10001220 エクスポート 73  PalGetInstanceHandle
10001230 エクスポート 46  PalEnd
10001FE0 エクスポート 135 PalRun
10002130 エクスポート 214 PalUninit
10002740 エクスポート 75  PalInit
100028B0 エクスポート 39  PalDefaultCreate
10002950 エクスポート 4   PalAnimationGetAnimationPoint
10002970 エクスポート 12  PalAnimationStartEx
10002990 エクスポート 14  PalAnimationStopEx
100029B0 エクスポート 9   PalAnimationResetEx
10002AB0 エクスポート 3   PalAnimationEx
10002B20 エクスポート 5   PalAnimationGetTime
10002B40 エクスポート 10  PalAnimationSetTime
10002B70 エクスポート 11  PalAnimationStart
10002B90 エクスポート 13  PalAnimationStop
10002BB0 エクスポート 8   PalAnimationReset
10002BF0 エクスポート 6   PalAnimationLoopIs
10002C20 エクスポート 7   PalAnimationRelease
10002D60 エクスポート 2   PalAnimation
10002EE0 エクスポート 26  PalButtonSetPos
10002F20 エクスポート 27  PalButtonSetReaction
10002F40 エクスポート 19  PalButtonCtrl
10002F80 エクスポート 24  PalButtonGetReactionEx
100030C0 エクスポート 23  PalButtonGetReaction
10003240 エクスポート 20  PalButtonDelete
100032A0 エクスポート 22  PalButtonEntryEx
10003300 エクスポート 21  PalButtonEntry
10003350 エクスポート 25  PalButtonRelease
100033B0 エクスポート 18  PalButtonCreateEx
10003400 エクスポート 17  PalButtonCreate
10003510 エクスポート 32  PalCursorSetPos
10003570 エクスポート 31  PalCursorSet
100036B0 エクスポート 38  PalDebugWindowDeleteLog
10003700 エクスポート 37  PalDebugWindowDelete
10003740 エクスポート 35  PalDebugSetData
10003840 エクスポート 34  PalDebugPrintf
10004300 エクスポート 36  PalDebugWindowCreate
10004F20 エクスポート 45  PalEffectTest
10005FD0 エクスポート 43  PalEffectEx
100060C0 エクスポート 40  PalEffect
100060E0 エクスポート 42  PalEffectEnableIs
100060F0 エクスポート 41  PalEffectEnable
10006110 エクスポート 44  PalEffectIs
10006390 エクスポート 49  PalErrorLogPuts
10006400 エクスポート 48  PalErrorLogPrintfBasic
10006500 エクスポート 47  PalErrorLogPrintf
100066B0 エクスポート 54  PalFileSetFilePointer
100069D0 エクスポート 53  PalFileCreateEx
10006B50 エクスポート 52  PalFileCreate
10007260 エクスポート 61  PalFontGetSize
10007A80 エクスポート 56  PalFontDrawText
10007B20 エクスポート 57  PalFontEnd
10007B70 エクスポート 55  PalFontBegin
10007C30 エクスポート 58  PalFontGetColor
10007C60 エクスポート 64  PalFontSetColor
10007C80 エクスポート 59  PalFontGetEffect
10007C90 エクスポート 65  PalFontSetEffect
10007CB0 エクスポート 60  PalFontGetFontSize
10007CC0 エクスポート 66  PalFontSetFontSize
10007CE0 エクスポート 62  PalFontGetType
10007CF0 エクスポート 67  PalFontSetType
10007D10 エクスポート 68  PalFontUnload
10007D40 エクスポート 63  PalFontLoad
10007DD0 エクスポート 51  PalExFontUnload
10007E00 エクスポート 50  PalExFontLoad
10007F00 エクスポート 69  PalFrameRateSet
10007F90 エクスポート 91  PalInputKeyCancel
10007FF0 エクスポート 76  PalInputClear
10008030 エクスポート 78  PalInputGetJoyOnEx
10008050 エクスポート 77  PalInputGetJoyOn
10008070 エクスポート 79  PalInputGetJoyPull
10008090 エクスポート 80  PalInputGetJoyPush
10008450 エクスポート 81  PalInputGetKeyEx
10008460 エクスポート 83  PalInputGetKeyOnEx
10008470 エクスポート 92  PalInputSetKeyOn
10008480 エクスポート 93  PalInputSetKeyPull
10008490 エクスポート 94  PalInputSetKeyPush
100084A0 エクスポート 82  PalInputGetKeyOn
100084B0 エクスポート 84  PalInputGetKeyPull
100084C0 エクスポート 85  PalInputGetKeyPush
100084D0 エクスポート 90  PalInputGetWheel
100084E0 エクスポート 87  PalInputGetMouseMoveY
100084F0 エクスポート 86  PalInputGetMouseMoveX
10008500 エクスポート 89  PalInputGetMouseY
10008510 エクスポート 88  PalInputGetMouseX
10008930 エクスポート 97  PalListGetDataCount
10008960 エクスポート 96  PalListGetData
100089B0 エクスポート 98  PalListPop
10008A10 エクスポート 100 PalListPushLast
10008A60 エクスポート 99  PalListPush
10008AA0 エクスポート 101 PalListRelease
10008B00 エクスポート 95  PalListCreate
10008B20 エクスポート 113 PalMemoryFree
10008B50 エクスポート 112 PalMemoryAlloc
10009EC0 エクスポート 102 PalMSpriteCheck
10009ED0 エクスポート 111 PalMSpriteUnlock
10009EF0 エクスポート 106 PalMSpriteLock
10009F10 エクスポート 109 PalMSpriteSetLoop
10009F40 エクスポート 104 PalMSpriteIsLoop
10009F60 エクスポート 103 PalMSpriteGetState
10009F80 エクスポート 107 PalMSpritePause
10009F90 エクスポート 110 PalMSpriteStop
10009FB0 エクスポート 108 PalMSpritePlay
10009FF0 エクスポート 105 PalMSpriteLoad
1000A100 エクスポート 119 PalNumberGetPlace
1000A130 エクスポート 115 PalNumberAlphaType
1000A160 エクスポート 118 PalNumberDrawCtrl
1000A190 エクスポート 136 PalSameNumberDraw
1000A230 エクスポート 117 PalNumberDraw
1000A420 エクスポート 121 PalNumberSetEx
1000A4F0 エクスポート 120 PalNumberSet
1000A5D0 エクスポート 116 PalNumberCreate
1000A620 エクスポート 125 PalRandomReset
1000A660 エクスポート 126 PalRandomSetSeed
1000A6B0 エクスポート 124 PalRandomGetSeed
1000A6F0 エクスポート 123 PalRandomEx
1000A730 エクスポート 122 PalRandom
1000AA80 エクスポート 131 PalRenderLockWait
1000AAD0 エクスポート 134 PalRenderUnlock
1000AAF0 エクスポート 130 PalRenderLock
1000AB10 エクスポート 129 PalRenderInsert
1000AB80 エクスポート 127 PalRenderGetActive
1000AB90 エクスポート 132 PalRenderSetActive
1000ABB0 エクスポート 133 PalRenderUninit
1000AC20 エクスポート 128 PalRenderInit
1000C0C0 エクスポート 139 PalSoundGetChannelCount
1000C1C0 エクスポート 142 PalSoundGetVolumeChannel
1000C1E0 エクスポート 141 PalSoundGetVolume
1000C210 エクスポート 153 PalSoundSetVolumeChannel
1000C250 エクスポート 152 PalSoundSetVolume
1000C410 エクスポート 151 PalSoundSetStartEnd
1000C440 エクスポート 140 PalSoundGetState
1000C460 エクスポート 148 PalSoundPlayIs
1000C4A0 エクスポート 145 PalSoundPause
1000C4E0 エクスポート 155 PalSoundStopFade
1000C560 エクスポート 154 PalSoundStop
1000C5D0 エクスポート 147 PalSoundPlayFade
1000C650 エクスポート 146 PalSoundPlay
1000C740 エクスポート 149 PalSoundRelease
1000C7C0 エクスポート 138 PalSoundCopy
1000C910 エクスポート 150 PalSoundReleaseEx
1000C9E0 エクスポート 144 PalSoundLoadVerEpeg
1000CB60 エクスポート 143 PalSoundLoad
1000D530 エクスポート 176 PalSpriteSetCenterOffset
1000D550 エクスポート 190 PalSpriteUnlock
1000D5A0 エクスポート 168 PalSpriteLock
1000D660 エクスポート 185 PalSpriteSetRenderMode
1000D680 エクスポート 181 PalSpriteSetOption
1000D980 エクスポート 175 PalSpriteSetBuffer
1000D9E0 エクスポート 156 PalSpriteBackBafferCopy
1000DA40 エクスポート 114 PalMixSpriteToSprite
1000DD00 エクスポート 29  PalCopySpriteToSpriteRGB
1000DE50 エクスポート 30  PalCopySpriteToSpriteRectRGB
1000DEA0 エクスポート 28  PalCopyFileToSpriteRGB
1000E3A0 エクスポート 189 PalSpriteToFile
1000E5E0 エクスポート 159 PalSpriteCreateBitmap
1000E5F0 エクスポート 178 PalSpriteSetInfo
1000E680 エクスポート 163 PalSpriteGetInfo
1000E710 エクスポート 162 PalSpriteGetForeground
1000E7A0 エクスポート 165 PalSpriteHit
1000E850 エクスポート 192 PalSpriteViewIs
1000E860 エクスポート 191 PalSpriteViewCtrl
1000E880 エクスポート 177 PalSpriteSetColor
1000E8A0 エクスポート 172 PalSpriteRectSetPos
1000E900 エクスポート 164 PalSpriteGetRect
1000E930 エクスポート 184 PalSpriteSetRect
1000E980 エクスポート 180 PalSpriteSetOffsetRect
1000E9C0 エクスポート 183 PalSpriteSetPrio
1000E9E0 エクスポート 187 PalSpriteSetRotateEx
1000EA10 エクスポート 186 PalSpriteSetRotate
1000EA30 エクスポート 188 PalSpriteSetScale
1000EA50 エクスポート 169 PalSpriteMoveOffsetPos
1000EA70 エクスポート 170 PalSpriteMovePos
1000EAA0 エクスポート 179 PalSpriteSetOffsetPos
1000EAC0 エクスポート 182 PalSpriteSetPos
1000EAE0 エクスポート 174 PalSpriteReleaseArray
1000EB30 エクスポート 173 PalSpriteRelease
1000EC00 エクスポート 171 PalSpritePaint
1000EC70 エクスポート 161 PalSpriteCreateTextEx
1000EDC0 エクスポート 160 PalSpriteCreateText
1000EFF0 エクスポート 157 PalSpriteCopy
1000F430 エクスポート 167 PalSpriteLoadMemory
1000F520 エクスポート 166 PalSpriteLoad
1000F600 エクスポート 158 PalSpriteCreate
100104C0 エクスポート 193 PalSystemPathSet
100109E0 エクスポート 204 PalTaskGetMessage
10010A00 エクスポート 210 PalTaskSetMessage
10010A10 エクスポート 202 PalTaskGetAppointTaskData
10010A20 エクスポート 207 PalTaskGetTaskData
10010A30 エクスポート 206 PalTaskGetSubTaskData
10010A50 エクスポート 211 PalTaskSetSubTaskData
10010A70 エクスポート 203 PalTaskGetGroundData
10010A90 エクスポート 195 PalTaskChangeNextImm
10010AC0 エクスポート 194 PalTaskChangeNext
10010AE0 エクスポート 205 PalTaskGetState
10010B00 エクスポート 196 PalTaskChangeState
10010B10 エクスポート 208 PalTaskReProcess
10010B50 エクスポート 209 PalTaskReset
10010BA0 エクスポート 201 PalTaskFreeAppointImm
10010BD0 エクスポート 200 PalTaskFreeAppoint
10010C10 エクスポート 199 PalTaskFree
10010C50 エクスポート 198 PalTaskCreateImm
10010CB0 エクスポート 197 PalTaskCreate
100110D0 エクスポート 213 PalThumbnailCreateMosaic
100112E0 エクスポート 212 PalThumbnailCreate
100113A0 エクスポート 224 PaltimeGetTime
10012510 エクスポート 215 PalVideoPlay
10012650 エクスポート 216 PalWait
10012700 エクスポート 217 PalWaitTime
100127C0 エクスポート 218 PalWaitTimeEx
10012C30 エクスポート 222 PalWindowModeIs
10012C40 エクスポート 219 PalWindowChangeMode
10012CD0 エクスポート 220 PalWindowGetPos
10012D00 エクスポート 223 PalWindowSetPos
10012DA0 エクスポート 221 PalWindowGetPosCenter
"""

lines = []
for line in text.splitlines():
    line = line.strip()
    if not line:
        continue
    m = re.match(r"^[0-9A-Fa-f]+\s+エクスポート\s+(\d+)\s+([A-Za-z0-9_]+)$", line)
    if not m:
        continue
    ordinal = m.group(1)
    name = m.group(2)

    if name == "PalVideoPlay":
        continue

    lines.append(
        f'#pragma comment(linker, "/export:{name}=PAL_orig.{name},@{ordinal}")'
    )

with open("exports.inc", "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
    f.write("\n")

print("exports.inc generated")