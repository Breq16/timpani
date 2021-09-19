import hid

for device in hid.enumerate():
    if device["vendor_id"] == 0x0451:
        print("Found it!")
        print(device)
        break
else:
    print("Nope")
