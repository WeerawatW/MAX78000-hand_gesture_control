# The MAX78000 hand gesture control
The main reference to the official warehouse of the board for training and deploying neural networks:`Github`

## Project Introduction
To control TV with hands or fingers, we want to create AI. to make YouTube and smart TV more convenient to use,This project will use MAX78000EVkit for deployment and test AI hand gesture control.
## MAX78000EVkit
![](BoardMAX78000EVkit.jpg)

### Purpose
This project create for use hand or fingers to control TV instead remote.
### Project process
The software part trains the neural network on the computer and deploys it to run on a single-chip microcomputer, using the CNN neural network accelerator built into the chip.

The main reference to the official warehouse of the board for training and deploying neural networks:`Github`

* `ai8x-training` Repository for training neural networks on computers:

https://github.com/MaximIntegratedAI/ai8x-training

* `ai8x-synthesis` Repository, used to convert the trained model file into C code:

https://github.com/MaximIntegratedAI/ai8x-synthesis

* `msdk` Warehouse, for writing microcontroller programs:

https://github.com/Analog-Devices-MSDK/msdk

* You can download  file for use training ai hand gesture control: https://github.com/WeerawatW/MAX78000-hand_gesture_control/releases/download/untagged-942b36396836df061b0d/project_file.zip or https://github.com/WeerawatW/MAX78000-hand_gesture_control/releases/download/ProjectFile/project_file.zip
## ai8x-training
Process datasets 
You can download only dataset here:

https://github.com/WeerawatW/MAX78000-hand_gesture_control/releases/download/untagged-a50fbdb1ade3ade8cfe3/finger_numbers.zip


After the download is complete, I organize the directory according to the following structure:
>
```
ai8x-training
 └─ data
   └─ Finger_Numbers
             ├─ processed
             │    ├─ test_info.csv
             │    ├─ train_info.csv
             │
             ├─ test
             └─ train
```
Place `finger_number.py` in dataset.
```
ai8x-training
  └─ dataset
        └─ finger_number.py
  
```
Place `train_Fingers_Numbers.sh` in ai8x-training.
```
ai8x-training
  └─ train_Fingers_Numbers.sh
``` 
Open terminal and type this command:
```
$cd ai8x-training
$source venv/bin/activate
$./train_Fingers_Numbers.sh
```
The trained model will be saved in a directory with these files:`logs`
```
 2023.06.07-141418.log
 best.pth.tar
 checkpoint.pth.tar
 qat_best.pth.tar
 qat_checkpoint.pth.tar
```
The naxt step in this will be used `qat_best.pth.tar` but before use we must rename `qat_best.pth.tar` to `ai85-finger-numbers-qat8.pth.tar`.

## ai8x-synthesis
Place `ai85-finger-numbers-qat8.pth.tar` in directory according to the following structure.
```
ai8x-synthesis
   └─ trained
        └─ ai85-finger-numbers-qat8.pth.tar
```
You will get `ai85-finger-numbers-qat8-q.pth.tar`

Place `quantize_Finger_Numbers.sh` and `gen-finger_numbers.sh` in directory according to the following structure.
```
ai8x-synthesis
   ├─ quantize_Finger_Numbers.sh
   └─ gen-finger_numbers.sh
```
Place `finger_numbers.yaml` in directory according to the following structure.
```
ai8x-synthesis
   └─ network
       └─ finger_numbers.yaml
```
Place `sample_fingers_number.npy` in directory according to the following structure.
```
ai8x-synthesis
   └─ tests
       └─ sample_fingers_number.npy
```
Type the following command:
```
$deactivate
$cd 
$cd ai8x-synthesis
$source venv/bin/activate
$./quantize_Finger_Numbers.sh
$./gen-finger_numbers.sh
```
After the run is completed, you will get the generated file in the directory `sdk/Examples/MAX78000/CNN/finger_numbers`
![](generated%20c%20code.jpg)


## Deployment on board MAX78000EVkit
-----------------------------------------------
-----------------------------------------------
## Conclusion
----------------------------------------------
---------------------------------------------
