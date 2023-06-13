# The MAX78000 hand gesture control
The main reference to the official warehouse of the board for training and deploying neural networks:`Github`

## Project Introduction
To control TV with hands or fingers, we want to create AI. to make  smart TV more convenient to use,This project will use MAX78000EVkit for deployment and test AI hand gesture control.
## MAX78000EVkit
![](BoardMAX78000EVkit.jpg)

**You can purchase and read more details here: https://www.analog.com/en/products/max78000.html**

The MAX78000 evaluation kit (EV kit) provides a platform for leveraging the capabilities of the MAX78000 to build new generations of artificial intelligence (AI) devices.

Onboard hardware includes a digital microphone, a gyroscope/ accelerometer, parallel camera module support and a 3.5in touch-enabled color TFT display. A secondary dis- play is driven by a power accumulator for tracking device power consumption over time. Uncommitted GPIO as well as analog inputs are readily accessible through 0.1in pin headers. Primary system power as well as UART access is provided by a USB Micro-B connector. A USB to SPI bridge provides rapid access to onboard memory, allow- ing large networks or images to load quickly.

https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/max78000evkit.html#eb-overview

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

* You can download our file for compare your result!: https://github.com/WeerawatW/MAX78000-hand_gesture_control/releases/download/untagged-942b36396836df061b0d/project_file.zip or https://github.com/WeerawatW/MAX78000-hand_gesture_control/releases/download/ProjectFile/project_file.zip

* But you can dowload file step by step too!.
## 1) ai8x-training
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

Dowload file here: https://github.com/WeerawatW/MAX78000-hand_gesture_control/blob/6c95af2d96c45ef934c73e30a5c9bdb86bf1a01e/ai8x-training/datasets/finger_number.py
```
ai8x-training
  └─ dataset
        └─ finger_number.py  
```
Place `train_Fingers_Numbers.sh` in ai8x-training.

Dowload file here: https://github.com/WeerawatW/MAX78000-hand_gesture_control/blob/2fb763b1f79c7f50ea98afd5a034290114651a4b/ai8x-training/train_Fingers_Numbers.sh
```
ai8x-training
  └─ train_Fingers_Numbers.sh
``` 

`python train.py --deterministic --print-freq 200  --epochs 100 --optimizer Adam --lr 0.001 --wd 0 --model ai85tinierssd --use-bias --momentum 0.9 --weight-decay 5e-4 --dataset Fingers_number --device MAX78000 --obj-detection --obj-detection-params parameters/obj_detection_params_svhn.yaml --batch-size 16 --qat-policy policies/qat_policy_svhn.yaml --validation-split 0 "$@"`

The meaning of each parameter is as follows:
| Parameters | describe parameter |
| ------------ | ------------------- |
| deterministic | Set the random number seed to produce repeatable training results |
| print-freq 200 | In each epech, how many training samples are printed once |
| pr-curves | Display the precision-recall curves Display the precision-recall curve |
| epochs 100 | the number of training times |
| optimizer Adam | optimizer |
| lr 0.001 | learning rate learning rate |
| wd 0: weight decay |
| model ai85tinierssd | model selection, the model definition is in the models folder |
| use-bias | use bias |
| momentum 0.9 | Momentum, a parameter of the Adam optimizer |
| weight-decay 5e-4 | Weight decay to prevent overfitting |
| dataset cat | dataset name, previously defined in the dataset loading file |
| device MAX78000 | MCU chip model |
| obj-detection | object detection |
| obj-detection-params | parameters/obj_detection_params_svhn.yaml target recognition training parameters |
| batch-size 16 | The number of samples passed into the neural network each time |
| qat-policy policies/qat_policy_svhn.yaml| policy for quantization parameters |
| validation-split 0 | Portion of training dataset to set aside for validation We have an independent validation set, no need to divide from the training set, here is set to 0 |

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

## 2) ai8x-synthesis
Place `ai85-finger-numbers-qat8.pth.tar` in directory according to the following structure.
```
ai8x-synthesis
   └─ trained
        └─ ai85-finger-numbers-qat8.pth.tar
```
You will get `ai85-finger-numbers-qat8-q.pth.tar` after run script `quantize_Finger_Numbers.sh`
 by using `ai85-finger-numbers-qat8-q.pth.tar`  for generate c code.

Place `sample_fingers_number.npy` in directory according to the following structure.

Dowload file here: https://github.com/WeerawatW/MAX78000-hand_gesture_control/blob/main/ai8x-synthesis/tests/sample_fingers_number.npy
```
ai8x-synthesis
   └─ tests
       └─ sample_fingers_number.npy
```

Place `finger_numbers.yaml` in directory according to the following structure.

Dowload file here: https://github.com/WeerawatW/MAX78000-hand_gesture_control/blob/main/ai8x-synthesis/network/finger_numbers.yaml
```
ai8x-synthesis
   └─ network
       └─ finger_numbers.yaml
```
Place `quantize_Finger_Numbers.sh` and `gen-finger_numbers.sh` in directory according to the following structure.

Dowload `quantize_Finger_Numbers.sh` here: https://github.com/WeerawatW/MAX78000-hand_gesture_control/blob/main/ai8x-synthesis/quantize_Finger_Numbers.sh

Dowload `gen-finger_numbers.sh` here: https://github.com/WeerawatW/MAX78000-hand_gesture_control/blob/main/ai8x-synthesis/gen-finger_numbers.sh
` python ai8xize.py --test-dir sdk/Examples/MAX78000/CNN --prefix finger_numbers --checkpoint-file trained/ai85-finger-numbers-qat8-q.pth.tar --config-file networks/finger_numbers.yaml --device MAX78000 --compact-data --mexpress --timer 0 --display-checkpoint --verbose --overlap-data --mlator --new-kernel-loader --overwrite --no-unload `

```
ai8x-synthesis
   ├─ quantize_Finger_Numbers.sh
   └─ gen-finger_numbers.sh
```

The meaning of each parameter is as follows:
| parameters | discript parameter |
| ----------- | ----------------- |
| test-dir sdk/Examples/MAX78000/CNN | save directory |
| prefix finger_numbers | saved folder name |
| checkpoint-file trained/ai85-finger-numbers-qat8-q.pth.tar | quantization format file in STEP 3 |
| networks/finger_numbers.yaml | network configuration file |
| device MAX78000 | the chip model of the microcontroller |
| compact-data | Use memcpy() to load input data to save code space (also enabled by default) |
| mexpress | use express kernel loading (also enabled by default) |
| timer 0 | use timer to time the inference (default: off, supply timer number) Whether to use timer to record the running time of CNN |
| display-checkpoint | show parsed checkpoint data |
| verbose | verbose output |
| overlap-data | allow output to override input |
| mlator | Use hardware to swap output bytes |
| new-kernel-loader | (also enabled by default) |
| overwrite | overwrite target (if present) |
| no-unload | disable cnn_unload() function |

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


## 3) Deployment on board MAX78000EVkit

Configure the environment
The environment here is very well configured, for the system, you do not need to clone the code from above, you can directly use the official exe program to install: https://www.analog.com/en/design-center/evaluation-hardware-and-software/software/software-download.html?swpart=sfw0010820a

After the installation is completed, it is necessary to configure a little, and then you can use it to complete compilation, burning and other operations.

### Getting Started with Visual Studio Code

The MSDK includes Visual Studio Code ("VS Code") support through the [VSCode-Maxim](https://github.com/Analog-Devices-MSDK/VSCode-Maxim) project.

This section walks through setup, opening, and running an example project with VS Code. This material is also available in video form targeting the MAX78000 in ["Understanding Artificial Intelligence Episode 8.5 - Visual Studio Code"](https://www.analog.com/en/education/education-library/videos/6313212752112.html). For complete documentation, see the[ Visual Studio Code](https://analog-devices-msdk.github.io/msdk/USERGUIDE/#visual-studio-code) section of this User Guide.

### Setup (VS Code)

The setup below only needs to be done once per MSDK https://analog-devices-msdk.github.io/msdk/USERGUIDE/#installation

1. Download and install Visual Studio Code for your OS here. https://code.visualstudio.com/Download
2. Launch Visual Studio Code.
3. Install the Microsoft https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
4. Use CTRL + SHIFT + P (or COMMAND + SHIFT + P on MacOS) to open the developer prompt.
5. Type "open user settings" and select the "Preferences: Open User Settings (JSON)" option.

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/20e5f755-0775-4ce4-991a-1b57210710ef)

6. Add the entries below to your user settings.json file. Change the MAXIM_PATH option to point to the MSDK installation.

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/a032b6ea-c7be-4ec2-9434-9f170f35746f)

For example, you might set "MAXIM_PATH":"C:/MaximSDK" on Windows, "MAXIM_PATH":"/home/username/MaximSDK" on Linux/MacOS, etc.

7. Save your changes to the file with CTRL + S and restart VS Code.

### Building and Running a Project (VS Code)

1. Launch Visual Studio Code.
2. Select File -> Open Folder...

 ![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/26e85ffe-1a60-41a6-850a-50632452c1fb)

3. Navigate to an example project for the target microcontroller in the Myproject folder and open it with Select Folder.

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/7d1f893e-f669-4584-825b-17bbc58f15f9)

4. VS Code will prompt for trust the first time. Select Trust folder and enable all features.

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/46531372-367e-4517-8b08-35ab3e00fdb4)

5. The opened project should look something like this.

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/825dd949-35eb-4d26-9166-1e5bf787f093)
6. Set the Board Support Package to match your evaluation platform. In VS Code, this is done by editing the .vscode/settings.json file and setting the "board" project configuration option.

See https://analog-devices-msdk.github.io/msdk/USERGUIDE/#board-support-packages for more details.

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/11061013-0817-4a1f-8b66-2568eee0d01c)

7. Save your changes to settings.json with CTRL+S.
8. Reload the VS Code window. After changing any options in settings.json, a reload is necessary to force it to re-index VS Code's Intellisense engine.
   VS Code can be conveniently reloaded with the Reload Window developer command accessed with CTRL + SHIFT + P (or COMMAND + SHIFT + P on MacOS).
   
![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/9a828a90-3a37-4156-8d75-2ea5dc900d8e)

9. Press the shortcut Ctrl+Shift+B to open the available Build Tasks (alternatively navigate to Terminal -> Run Build task...).

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/e5c88b96-3e88-4a9e-8a87-622371ae240d)

10. Run the "build" task to compile the project for the configured Target Microcontroller and BSP. Notice that the TARGET and BOARD Build Configuration Variables are set on the command line. The program binary is successfully compiled into the .elf program binary in the build sub-folder of the project.

            ----------image--------
11. Connect a debug adapter between the host PC and the evaluation platform. Detailed instructions on this hardware setup can be found in the evaluation platform's Datasheet and Quick-Start Guide, which are available on its https://www.analog.com/en/index.html product page.
12. Run the flash build task. Running this task will automatically build the project if needed, flash the program binary, and halt the program execution to await a debugger connection.

      ----------image--------
13. Open the Run and Debug window (CTRL+SHIFT+D) and launch the debugger (F5).

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/e3483859-fc48-4351-9f87-28fd293e1acb)

14. Verify the program counter enters main successfully.

     ----------image--------
     
15. Press Continue (F5) to run the program.

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/1d5373c8-65a6-40e3-8839-b66c01078ab2)

![image](https://github.com/WeerawatW/MAX78000-hand_gesture_control/assets/136284844/980c31ab-982d-49c5-8f43-b9fe77cc7035)


16. Exercise the debugger and press stop to disconnect when finished.


## Conclusion
----------------------------------------------
---------------------------------------------
