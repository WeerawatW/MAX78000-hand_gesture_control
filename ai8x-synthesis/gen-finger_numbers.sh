 #!/bin/sh
DEVICE="MAX78000"
TARGET="sdk/Examples/$DEVICE/CNN"
COMMON_ARGS="--device $DEVICE --timer 0 --display-checkpoint --verbose"

#python ai8xize.py --test-dir $TARGET --prefix finger_numbers --checkpoint-file trained/ai85-finger-numbers-qat8-q.pth.tar --config-file networks/hand.yaml --fifo --softmax $COMMON_ARGS "$@"
python ai8xize.py --test-dir sdk/Examples/MAX78000/CNN --prefix finger_numbers --checkpoint-file trained/ai85-finger-numbers-qat8-q.pth.tar --config-file networks/finger_numbers.yaml --device MAX78000 --compact-data --mexpress --timer 0 --display-checkpoint --verbose --overlap-data --mlator --new-kernel-loader --overwrite --no-unload
