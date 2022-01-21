<#
.DESCRIPTION
    A script that allows to go from a sketch (arduino) folder to platformio folder
    Requires a few global variables to be correct



.CREATION
    Twenne Elffers
    twenne.elffers@hva.nl
    11/12/2021
    made for hackableESP project

#>


$global:currentDir = (Get-Location).toString()
$global:SketchDirName = ($currentDir+"\hackableEspDevice\")
$global:platfomrmioDirName = ($currentDir+"\HackableEspDevicePlatformio")
$global:platformioIniName = ($currentDir+"\platformio.ini")
$global:mklinkName = ($currentDir+"\mklink.cmd")
$global:platformioHFilename = "platformio.h"

$platformioDirCheck= Test-Path $platfomrmioDirName
$sketchCheck = Test-Path $SketchDirName
$mklinkCheck = Test-Path $mklinkName




Function createPlatformioDir(){
        <#
    .Description
    Creates all dirs that platformio needs which are:
    ./<platformiodir>
    ./<platformiodir>/src/
    ./<platformiodir>/data/
    src and data are done by create platformioSub function
    #>
    <#
    .dependency
    Creates all dirs that platformio needs which are:
    ./<platformiodir>
    ./<platformiodir>/src/
    ./<platformiodir>/data/
    src and data are done by create platformioSub function
    #>

    $platformioDirCheck= Test-Path $platfomrmioDirName
    if ($platformioDirCheck){
        write-host "Platformio Dir found"
        createPlatformioSub
    }
    elseif ($platformioDirCheck-eq$false){
        write-host "Platformio Dir not found"
        New-Item -Path ($platfomrmioDirName) -ItemType Directory | Out-Null #Create platformio dir
        write-host "`tPlatformio Dir created"
        createPlatformioSub
    }
}
function createPlatformioSub() {
    <#
    .Description
    Creates all sub dirs that platformio needs which are:
    ./src
    ./data
    .OUTPUTS
    #>
    $srcFolderCheck = Test-Path ($platfomrmioDirName+"\src")
    $dataFolderCheck = Test-path ($platfomrmioDirName+"\data")
    if ($srcFolderCheck-eq$False){ #if src doesn't exits
        New-Item -Path ($platfomrmioDirName+"\src") -ItemType Directory | Out-Null
        write-host "`tsrc created"
    }
    if ($dataFolderCheck -eq $False){ # if data doesn't exist
        New-Item -Path ($platfomrmioDirName+"\data") -ItemType Directory| Out-Null
        write-host "`tdata created"
    }
}
function getDir($dir){
    <#
    .Description
    get all files in sketch and sketch/data
    .PARAMETER Dir
    must contain the sketch dir which files need to be read from
    .OUTPUTS
    array containing System.IO.FileInfo files from input dir and dir/data 
    #>
    $dirList = @() #creat array that will contain all files
    $dirFiles = Get-ChildItem -File $dir #get all files from sketch (no Directories)
    foreach ($file in $dirFiles){
        $dirList += $file
    }
    $dirFiles = Get-ChildItem -File ($dir+"\data") #get all files from sketch data (no Directories)
    foreach ($file in $dirFiles){
        $dirList += $file
    }
    return $dirList
}
function NewMainFile ($file, $outfile,$platformioH) {
    $platformioIncludeCounter = 0
    $platformioHContents = "
    #ifndef PLATFORMIO_H
    #define PLATFORMIO_H`n"
    $platformioHContentsEnd = "`n#endif"
    $fileContents = Get-Content $file
    $mainfile = ""
    foreach ($line in $fileContents){
        if ($line.toLower() -match "(void|String|bool)\s\w+\("){
            $mainfile += ($line +"`n")
            if ($line -match "({|{\s*})$"){
                $line = $line.replace("{",";")
                while ($line -match "\)(\s+);"){
                    $line = $line.replace(" ;", ";")
                }
            }
            $platformioHContents += ($line +"`n")
            write-host $line
        }
        elseif ($line.tolower() -match '^#include "'){ #TODO:Fix the dubble add
            if ($line.tolower() -match ('^#include "'+$platformioHFilename+'"')){
                write-host (('^#include "'+$platformioHFilename+'"'))
                write-host $line.tolower()
                $platformioIncludeCounter += 1
            }
            elseif ($platformioIncludeCounter -eq 0 ){
                $mainfile += '#include "'+$platformioHFilename+'"'+"`n"+($line +"`n")
                $platformioIncludeCounter += 1
            }
            else {
                $mainfile += ($line +"`n")
            }
        }
        else {
            $mainfile += ($line +"`n")
        }
    }
    $platformioHContents += $platformioHContentsEnd
    Set-Content -Path $outfile -Value $mainfile
    Set-Content -Path $platformioH -Value $platformioHContents 
}
function CreateSymLinkFile(){
        <#
    .Description
    fills the mklink.cmd for symlink creation

    #>
    if (Test-Path $platformioIniName){
        Write-output ('mklink "'+$platfomrmioDirName+'\platformio.ini'+'" "'+$platformioIniName+'"') | Out-File $mklinkName -Append -Encoding "UTF8"
    }
    $SketchFiles = getDir $SketchDirName
    foreach ($file in $SketchFiles){
        if ($file.fullName -match ".*\\data\\.*"){ #check for all files in data folder
            #write-host $file.fullName
                Write-output ('mklink "'+$platfomrmioDirName+"\data\"+$file.name+'" "'+$file.fullName+'"') | Out-File $mklinkName -Append -Encoding "UTF8"
        }
        elseif ($file.FullName -match ".*ino"){ #Check for the ino sketch file use main.cpp for easier identification
                Write-output ('mklink "'+$platfomrmioDirName+"\src\main.cpp"+'" "'+$file.fullName+'"') | Out-File $mklinkName -Append -Encoding "UTF8"
        }
        else{
                Write-output ('mklink "'+$platfomrmioDirName+'\src\'+$file.name+'" "'+$file.fullName+'"') | Out-File $mklinkName -Append -Encoding "UTF8"
        }
    }
}
Function CopyFiles(){
        <#
    .Description
    copies all files from sketch to platformio folder

    #>
    if (Test-Path $platformioIniName){
        copy-item $platformioIniName ($platfomrmioDirName+'\platformio.ini')
    }
    $SketchFiles = getDir $SketchDirName
    foreach ($file in $SketchFiles){
        if ($file.fullName -match ".*\\data\\.*"){ #check for all files in data folder
            copy-item $file.Fullname ($platfomrmioDirName+'\data\'+$file.name)
        }
        elseif ($file.FullName -match ".*ino$"){ #Check for the ino sketch file use main.cpp for easier identification
            NewMainFile -file $file.Fullname -outfile ($platfomrmioDirName+'\src\main.cpp') -platformioH ($platfomrmioDirName+'\src\'+$platformioHFilename)
            #copy-item $file.Fullname ($platfomrmioDirName+'\src\main.cpp')
        }
        else{
            copy-item $file.Fullname ($platfomrmioDirName+'\src\'+$file.name)
        }
    }
}


$global:symbolicLinkCheck = $false  #change this to $true if you want a mklink.cmd for editing does require platformio dir to be empty
                            #change this to $false if you want to just copy all files

$doCheck = $true # do check to see if user would like copy or symbolic
if ($doCheck){
    $symbolicLinkInput = Read-Host -Prompt "would you would like to create symbolic links or just copy the files ((S)ymbolic/(C)opy/(F)ix)`nSymbolic links updates the files in the home dir as you edit them    ((S)ymbolic/(C)opy/(F)ix)`nOr would you like to fix the main file                               ((S)ymbolic/(C)opy/(F)ix)"
    if ($symbolicLinkInput.ToLower()-eq "c" -or $symbolicLinkInput.ToLower()-eq "copy"){
        $symbolicLinkCheck = $false
    }
    elseif ($symbolicLinkInput.ToLower()-eq "s" -or $symbolicLinkInput.ToLower()-eq "symbolic"){
        $symbolicLinkCheck = $true
    }
    elseif ($symbolicLinkInput.ToLower()-eq "f" -or $symbolicLinkInput.ToLower()-eq "fix"){
        $dirFiles = getDir $SketchDirName
        foreach ($file in $dirFiles){
            if ($file.FullName -match ".*ino$"){ #Check for the ino sketch file use main.cpp for easier identification
                write-host $file.FullName
                NewMainFile -file $file.Fullname -outfile ($platfomrmioDirName+'\src\main.cpp') -platformioH ($platfomrmioDirName+'\src\'+$platformioHFilename)
                #copy-item $file.Fullname ($platfomrmioDirName+'\src\main.cpp')
            }
        }
        exit
    }
    else{ #if input is not understood stop program
        write-host "input not understood"
        pause
        exit
    }
}

#first check if needed files are there
if($sketchCheck){
    write-host "Sketch dir found"
    if ($platformioDirCheck){ #if exist check sub folders
        write-host "Platformio Dir found"
        createPlatformioSub
    }
    else{ # if does not exist create both
        createPlatformioDir
    }
}
else { #if sketch folder does not exits exit the file
    write-host "SketchDir not found`n You are most likely in the wrong location"
    pause
    exit
}


if ($symbolicLinkCheck){ # if the user wants symbolic files
    if ($mklinkCheck-eq $false){
        New-Item -Path ($mklinkName) -ItemType File |Out-Null
    }
    write-output "" |Out-File $mklinkName  -Encoding "UTF8"
    CreateSymLinkFile
    Write-output ("pause") | Out-File $mklinkName -Append -Encoding "UTF8"
    write-host "Created mklink.cmd"


    $userInput = Read-Host -Prompt"would you like to execute mklink.cmd?(requires admin) (Y/n) "
    if ($userInput.ToLower()-eq "y"){
        write-host "I REALLY WANT TO DO THIS"
        start-process $mklinkName -verb Runas
    }
    elseif ($userInput.ToLower()-eq "n"){
        exit
    }
}
elseif($symbolicLinkCheck -eq $false){ # if user doesn't want symbolic link copy files

    CopyFiles
    write-host "Files coppied"
}
Pause
