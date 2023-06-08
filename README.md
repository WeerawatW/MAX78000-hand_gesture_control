# The MAX78000 hand gesture control

* you can download  file for use training ai hand gesture control: https://github.com/WeerawatW/MAX78000-hand_gesture_control/releases/download/untagged-942b36396836df061b0d/project_file.zip. by this link.
## ai8x-training
Process datasets 

After the download is complete, I organize the directory according to the following structure:
>
```
ai8x-training
 └─ data
   └─ finger_number
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
