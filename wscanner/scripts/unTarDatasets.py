#!/usr/bin/env python
import os
import math
import shlex
import subprocess
import time
import tarfile
import shutil

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--inputDir", type = str, help = "Path to folder with files")
parser.add_argument("--outputDir", default = "root-files-Grid", type = str, help = "Path to folder with files")

args = parser.parse_args()

globalScanType = ""
def main():
    tarFileList = getFileListWithPattern(args.inputDir, "JobSummary.tgz.gz")


    tmpDir = "gridExtractTmpDir"


    for fileName in tarFileList:
        cTarfile = tarfile.open(fileName)
        cTarfile.extractall(tmpDir)
        cTarfile.close()


    # clean up the directory
    copyDirTree(tmpDir + "/root-files", args.outputDir)
    shutil.rmtree(tmpDir, False)


def copyDirTree(root_src_dir,root_dst_dir):
    """
    Copy directory tree. Overwrites also read only files.
    :param root_src_dir: source directory
    :param root_dst_dir:  destination directory
    """
    for src_dir, dirs, files in os.walk(root_src_dir):
        dst_dir = src_dir.replace(root_src_dir, root_dst_dir, 1)
        if not os.path.exists(dst_dir):
            os.makedirs(dst_dir)
        for file_ in files:
            src_file = os.path.join(src_dir, file_)
            dst_file = os.path.join(dst_dir, file_)
            if os.path.exists(dst_file):
                try:
                    os.remove(dst_file)
                except PermissionError as exc:
                    os.chmod(dst_file, stat.S_IWUSR)
                    os.remove(dst_file)

            shutil.copy(src_file, dst_dir)

def getFileListWithPattern(dirName, pattern):
    fileList = []
    for root, dirs, files in os.walk(dirName):
        for name in files:
            if pattern not in name: continue
            fileList.append(os.path.join(root, name))
    return fileList


def getSubFolderList(dirName):
    fileList = []
    for files in os.listdir(dirName):
        if os.path.isdir(dirName + "/" + files):
            fileList.append(files)
            pass
        pass
    
    fileList.sort()
    return fileList

def getRootFileList(dirName):
    fileList = []
    for fileName in os.listdir(dirName):
        if(fileName.endswith(".root")):
            fileList.append(dirName + "/" + fileName);
    return fileList

def getRootFileListSizeCheck(dirName):
    fileList = []
    for fileName in os.listdir(dirName):
        if(fileName.endswith(".root")):
            if(os.stat(dirName + "/" + fileName).st_size < 100): continue
            fileList.append(dirName + "/" + fileName);
    return fileList

def checkIfSubfolder(dirName):
    for fileName in os.listdir(dirName):
        if os.path.isdir(dirName + "/" + fileName):
            return True
    return False

    




if __name__ == "__main__":
    main()
