# Connection attempt

```log
| RSSI: -40 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] 46:9c:34:91:7f:12 | RSSI: -67 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 fc 35 82 10 02 2a 04
[SCAN] 8c:f6:81:fe:8a:f6 | RSSI: -70 | Name: "WB237202" | Services: 331a36f5-2459-45ea-9d95-6142f0c4b307
[SCAN] 59:6c:a9:9c:d6:a4 | RSSI: -79 | Name: "" | MFR (9 bytes): 4c 00 10 05 00 1c 66 75 3b
[SCAN] 68:b2:01:fb:50:4f | RSSI: -40 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 0f 98 75 79 4c 2c 23
[SCAN] f0:f5:bd:31:35:66 | RSSI: -82 | Name: "EF-R3BP0410" | MFR (26 bytes): b5 b5 13 52 36 35 35 5a 45 42 34 58 47 42 50 30 34 31 30 46 00 01 00 00 3e 33
[SCAN] 58:0b:2d:03:a9:6c | RSSI: -71 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 [ 16:05:00 2026-02-21 ]❯ pio device monitor
--- Terminal on /dev/cu.usbserial-410 | 115200 8-N-1
--- Available filters and text transformations: debug, default, direct, esp32_exception_decoder, hexlify, log2file, nocontrol, printable, send_on_enter, time
--- More details at https://bit.ly/pio-monitor-filters
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H

=== Giant E-Bike BLE Explorer ===
Commands (via Serial):
  s           - Start BLE scan
  c <address> - Connect to device (e.g., c AA:BB:CC:DD:EE:FF)
  d           - Disconnect
  r           - Re-discover and read all services/characteristics
  n           - Subscribe to all notifications
  h           - Show this help

[BLE] Starting scan for 10 seconds...
[SCAN] 4e:04:cd:ca:ba:1e 00 c3 1b 58 16 08 00 f9 1c c9 65 11 d5 5b
[SCAN] 58:7d:8d:b8:a9:f3 | RSSI: -69 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 00 1b 5c 10 02 28 04
[SCAN] 5d:72:52:92:a5:89 | RSSI: -57 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 6b:d6:31:fb:23:bc | RSSI: -57 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 68 1b 58 16 08 00 af 51 d2 1f 9b fc 05
[SCAN] c4:46:c7:cd:94:01 | RSSI: -73 | Name: "" | MFR (21 bytes): 2d 01 04 00 01 31 05 01 e5 31 e2 32 04 40 d5 00 00 00 00 00 00
[SCAN] 75:e0:56:f4:24:ff | RSSI: -48 | Name: "" | MFR (9 bytes): 4c 00 10 05 2c 18 00 ae 06
[SCAN] d2:d2:04:c9:06:cd | RSSI: -71 | Name: "S16 31CF LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 6e:9b:4d:50:a8:a7 | RSSI: -60 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 49 1b 58 16 08 00 6e 96 1a 95 15 f9 54
[SCAN] 55:49:5c:cf:48:63 | RSSI: -60 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 49:2b:69:75:0c:6e | RSSI: -59 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 47 87 87 10 02 29 04
[SCAN] 33:0b:2c:20:2a:d4 | RSSI: -75 | Name: "" | MFR (29 bytes): 06 00 01 09 20 22 cc df e2 06 69 54 c1 b0 47 60 05 b0 ef 15 81 37 61 b7 10 85 74 e1 f5
[SCAN] 80:6f:b0:23:f4:a0 | RSSI: -81 | Name: "S80e70c8bf41ad7a5C" | MFR (25 bytes): 4c 00 02 15 74 27 8b da b6 44 45 20 8f 0c 72 0e af 05 99 35 00 00 7a 0d c5  | Services: 0x1122
[SCAN] 61:5a:c4:e3:61:f1 | RSSI: -69 | Name: "" | MFR (9 bytes): 4c 00 10 05 4b 1c 69 4e 49
[SCAN] 18:c2:3c:95:e8:01 | RSSI: -48 | Name: "Aqara-0804-f2f" | MFR (15 bytes): 27 0b 08 10 00 f2 f1 15 f0 80 4a ff 9f 03 c0  | Services: 0xfcb9
[SCAN] c2:0a:b5:b7:ab:40 | RSSI: -70 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] 46:84:0d:e9:a7:e2 | RSSI: -83 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] 43:71:47:54:2e:1f | RSSI: -61 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 a8 0a 00 00 ee 1b 58 16 08 00 3d 86 e6 2f c1 fe e1
[SCAN] 6a:e9:98:6a:01:5b | RSSI: -83 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] 35:6f:f3:af:c4:50 | RSSI: -61 | Name: "" | MFR (12 bytes): 4c 00 13 08 5a a9 f1 59 3a 4e 25 00
[SCAN] 6b:13:cc:81:c4:b3 | RSSI: -70 | Name: "" | MFR (9 bytes): 4c 00 10 05 13 18 ba 8d d6
[SCAN] f8:5e:d1:b8:77:37 | RSSI: -82 | Name: "HomeHeatS" | MFR (3 bytes): f1 04 10
[SCAN] 42:3c:bb:5e:8e:16 | RSSI: -67 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9a 0a 00 00 b1 1b 58 16 08 00 ab 8d 17 ec 79 68 4a
[SCAN] 1f:02:e3:9f:09:01 | RSSI: -67 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a ed 67 89 ad 09 fc 00
[SCAN] 13:bc:2f:75:5d:21 | RSSI: -71 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a 23 c8 2b 58 e0 63 00
[SCAN] 67:df:31:de:88:bd | RSSI: -77 | Name: "" | MFR (9 bytes): 4c 00 10 05 27 98 54 60 c4
[SCAN] de:de:13:3e:bf:1c | RSSI: -70 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] d3:3b:04:bd:11:4d | RSSI: -77 | Name: "" | MFR (6 bytes): 4c 00 12 02 25 03
[SCAN] 58:d3:49:e1:d1:28 | RSSI: -79 | Name: "" | MFR (9 bytes): 4c 00 10 05 0e 14 1a ec 5d
[SCAN] 38:29:35:f9:21:43 | RSSI: -60 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a c8 5c bd 7b 69 ba 00
[SCAN] 68:27:37:7d:32:8e | RSSI: -84 | Name: "" | MFR (26 bytes): 75 00 42 04 01 80 60 68 27 37 7d 32 8e 6a 27 37 7d 32 8d 01 00 00 00 00 00 00
[SCAN] e3:72:38:30:88:7b | RSSI: -44 | Name: "" | MFR (21 bytes): 4c 00 07 11 06 0b b0 5a 23 a7 49 1c e3 82 2a 74 62 9e f9 79 c4
[SCAN] dd:1e:5f:14:76:8e | RSSI: -78 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] 63:40:59:46:b8:5b | RSSI: -72 | Name: ""
[SCAN] e7:a7:8e:13:94:9d | RSSI: -89 | Name: "S17 695E LE" | MFR (19 bytes): a7 05 06 00 12 00 29 00 c8 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 7d:c4:50:79:e9:94 | RSSI: -72 | Name: "" | MFR (25 bytes): 4c 00 02 15 50 76 5c b7 d9 ea 4e 21 99 a4 fa 87 96 13 a4 92 6e 0b bb 69 ce
[SCAN] 70:49:c4:53:83:c5 | RSSI: -74 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 05 1b 58 16 08 00 18 1b ca 8c 9b 1c af
[SCAN] d9:9f:34:c5:b4:d0 | RSSI: -63 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 03
[SCAN] 65:0d:06:a3:58:7e | RSSI: -74 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] e5:e7:d1:78:30:fc | RSSI: -81 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] 49:ed:e8:2c:42:dd | RSSI: -78 | Name: "" | MFR (28 bytes): 2d 01 02 00 01 10 dc 56 df 5d ed 6b 49 6a aa e2 55 da 99 c9 2d da ff 0d d5 06 25 c5
[SCAN] de:7a:54:e1:83:d4 | RSSI: -34 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] fa:f0:11:6f:24:67 | RSSI: -75 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] d0:f0:c4:bc:aa:9f | RSSI: -63 | Name: "" | MFR (6 bytes): 4c 00 12 02 14 03
[SCAN] 4e:04:cd:ca:ba:1e | RSSI: -43 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] 46:9c:34:91:7f:12 | RSSI: -67 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 fc 35 82 10 02 2a 04
[SCAN] 8c:f6:81:fe:8a:f6 | RSSI: -72 | Name: "WB237202" | Services: 331a36f5-2459-45ea-9d95-6142f0c4b307
[SCAN] 6e:2f:ce:04:83:62 | RSSI: -87 | Name: "" | MFR (11 bytes): 4c 00 10 07 3a 1f 7f f3 fa 4b 18
[SCAN] 7b:bb:bb:58:a4:11 | RSSI: -85 | Name: "" | MFR (8 bytes): e0 00 00 23 ca 7c d1 9d  | Services: 0xfe9f
[SCAN] 58:0b:2d:03:a9:6c | RSSI: -71 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 c3 1b 58 16 08 00 f9 1c c9 65 11 d5 5b
[SCAN] 58:7d:8d:b8:a9:f3 | RSSI: -60 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 00 1b 5c 10 02 28 04
[SCAN] 71:56:33:da:8d:72 | RSSI: -73 | Name: "" | MFR (10 bytes): 4c 00 10 06 31 1e ce c3 3b 87
[SCAN] f0:9d:02:97:34:af | RSSI: -84 | Name: "" | MFR (6 bytes): 4c 00 12 02 54 03
[SCAN] 68:b2:01:fb:50:4f | RSSI: -41 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 0f 98 75 79 4c 2c 23
[SCAN] f7:31:b7:bd:4f:23 | RSSI: -41 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] 5e:1a:a6:4b:d1:70 | RSSI: -89 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 6e 0a 00 00 d0 1b 58 16 08 00 72 23 9a ac b8 2c 0c
[SCAN] 6a:ef:f4:28:99:54 | RSSI: -67 | Name: "" | MFR (9 bytes): 4c 00 10 05 0e 18 a3 69 82
[SCAN] 6d:f5:d1:bf:66:17 | RSSI: -49 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 a0 d8 b9 ab a5 5a 95
[SCAN] c3:df:bb:ae:dc:8b | RSSI: -70 | Name: "vuart:ktunnel" | MFR (4 bytes): 27 0b 00 00
[SCAN] e4:e6:13:1f:f2:36 | RSSI: -56 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] 6b:d6:31:fb:23:bc | RSSI: -62 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 68 1b 58 16 08 00 af 51 d2 1f 9b fc 05
[SCAN] c4:46:c7:cd:94:01 | RSSI: -79 | Name: "" | MFR (21 bytes): 2d 01 04 00 01 31 05 01 e5 31 e2 32 04 40 d5 00 00 00 00 00 00
[SCAN] f0:f5:bd:31:35:66 | RSSI: -82 | Name: "EF-R3BP0410" | MFR (26 bytes): b5 b5 13 52 36 35 35 5a 45 42 34 58 47 42 50 30 34 31 30 46 00 01 00 00 3e 33
[SCAN] 62:e9:e8:77:c9:b3 | RSSI: -82 | Name: "" | MFR (10 bytes): 4c 00 10 06 3d 1e 18 8f ba f0
[SCAN] d2:d2:ba:e7:21:5d | RSSI: -72 | Name: "S16 9C38 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 50:b7:50:e3:93:4b | RSSI: -55 | Name: "" | MFR (11 bytes): 4c 00 10 07 3f 1f a0 e1 b0 0d 48
[SCAN] 60:7d:fa:21:65:9d | RSSI: -73 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 9d 46 dc 83 ee 3f 1d
[SCAN] 4a:7d:07:a0:e7:a9 | RSSI: -68 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] d2:d2:3e:21:65:1f | RSSI: -77 | Name: "S16 9C77 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 75:e0:56:f4:24:ff | RSSI: -41 | Name: "" | MFR (9 bytes): 4c 00 10 05 2c 18 00 ae 06
[SCAN] 7a:c4:29:36:27:3f | RSSI: -38 | Name: "" | MFR (10 bytes): 4c 00 10 06 0b 1d f8 e4 4f 48
[SCAN] 55:49:5c:cf:48:63 | RSSI: -70 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 33:0b:2c:20:2a:d4 | RSSI: -73 | Name: "" | MFR (29 bytes): 06 00 01 09 20 22 cc df e2 06 69 54 c1 b0 47 60 05 b0 ef 15 81 37 61 b7 10 85 74 e1 f5
[SCAN] 4e:58:41:ac:84:f0 | RSSI: -71 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 a6 94 b5 10 02 2c 04
[SCAN] 80:6f:b0:23:f4:a0 | RSSI: -75 | Name: "S80e70c8bf41ad7a5C" | MFR (25 bytes): 4c 00 02 15 74 27 8b da b6 44 45 20 8f 0c 72 0e af 05 99 35 00 00 7a 0d c5  | Services: 0x1122
[SCAN] 20:57:9e:66:d2:2c | RSSI: -58 | Name: "SKY0766"
[SCAN] d2:d2:04:c9:06:cd | RSSI: -69 | Name: "S16 31CF LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 35:6f:f3:af:c4:50 | RSSI: -63 | Name: "" | MFR (12 bytes): 4c 00 13 08 5a a9 f1 59 3a 4e 25 00
[SCAN] 61:5a:c4:e3:61:f1 | RSSI: -78 | Name: "" | MFR (9 bytes): 4c 00 10 05 4b 1c 69 4e 49
[SCAN] ff:8e:3b:bf:10:56 | RSSI: -90 | Name: "" | MFR (6 bytes): 4c 00 12 02 35 01
[SCAN] 7e:12:a8:fa:46:54 | RSSI: -80 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 4c bb 82 f7 2d a9 41
[SCAN] 6a:e9:98:6a:01:5b | RSSI: -80 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] 42:3c:bb:5e:8e:16 | RSSI: -67 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9a 0a 00 00 b1 1b 58 16 08 00 ab 8d 17 ec 79 68 4a
[SCAN] 59:6c:a9:9c:d6:a4 | RSSI: -70 | Name: "" | MFR (9 bytes): 4c 00 10 05 00 1c 66 75 3b
[SCAN] 67:df:31:de:88:bd | RSSI: -73 | Name: "" | MFR (9 bytes): 4c 00 10 05 27 98 54 60 c4
[SCAN] fb:40:f0:f2:c8:58 | RSSI: -90 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] f8:5e:d1:b8:77:37 | RSSI: -69 | Name: "HomeHeatS" | MFR (3 bytes): f1 04 10
[SCAN] 6e:9b:4d:50:a8:a7 | RSSI: -60 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 49 1b 58 16 08 00 6e 96 1a 95 15 f9 54
[SCAN] 1f:02:e3:9f:09:01 | RSSI: -78 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a ed 67 89 ad 09 fc 00
[SCAN] 13:bc:2f:75:5d:21 | RSSI: -71 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a 23 c8 2b 58 e0 63 00
[SCAN] 38:29:35:f9:21:43 | RSSI: -59 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a c8 5c bd 7b 69 ba 00
[SCAN] 63:40:59:46:b8:5b | RSSI: -75 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 63:40:59:46:b8:5b | RSSI: -75 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 7d:c4:50:79:e9:94 | RSSI: -67 | Name: "" | MFR (25 bytes): 4c 00 02 15 50 76 5c b7 d9 ea 4e 21 99 a4 fa 87 96 13 a4 92 6e 0b bb 69 ce
[SCAN] 68:27:37:7d:32:8e | RSSI: -85 | Name: "" | MFR (26 bytes): 75 00 42 04 01 80 60 68 27 37 7d 32 8e 6a 27 37 7d 32 8d 01 00 00 00 00 00 00
[SCAN] 43:71:47:54:2e:1f | RSSI: -64 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 a8 0a 00 00 ee 1b 58 16 08 00 3d 86 e6 2f c1 fe e1
[SCAN] e4:88:dc:a8:e8:e2 | RSSI: -80 | Name: "DoorLocker" | MFR (15 bytes): 27 0b 28 08 03 ae a1 15 f2 80 2a 12 fc 22 80  | Services: 0xfcb9
[SCAN] de:de:13:3e:bf:1c | RSSI: -63 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] d3:3b:04:bd:11:4d | RSSI: -81 | Name: "" | MFR (6 bytes): 4c 00 12 02 25 03
[SCAN] 70:49:c4:53:83:c5 | RSSI: -73 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 05 1b 58 16 08 00 18 1b ca 8c 9b 1c af
[SCAN] 49:ed:e8:2c:42:dd | RSSI: -72 | Name: "" | MFR (28 bytes): 2d 01 02 00 01 10 dc 56 df 5d ed 6b 49 6a aa e2 55 da 99 c9 2d da ff 0d d5 06 25 c5
[SCAN] 65:0d:06:a3:58:7e | RSSI: -72 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] e3:72:38:30:88:7b | RSSI: -54 | Name: "" | MFR (21 bytes): 4c 00 07 11 06 0b b0 5a 23 a7 49 1c e3 82 2a 74 62 9e f9 79 c4
[SCAN] e7:a7:8e:13:94:9d | RSSI: -85 | Name: "S17 695E LE" | MFR (19 bytes): a7 05 06 00 12 00 29 00 c8 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] dd:1e:5f:14:76:8e | RSSI: -70 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] 8c:f6:81:fe:8a:f6 | RSSI: -77 | Name: "WB237202" | Services: 331a36f5-2459-45ea-9d95-6142f0c4b307
[SCAN] 6b:13:cc:81:c4:b3 | RSSI: -63 | Name: "" | MFR (9 bytes): 4c 00 10 05 13 18 ba 8d d6
[SCAN] 5d:72:52:92:a5:89 | RSSI: -57 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] e7:da:d0:88:66:c1 | RSSI: -49 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 03
[SCAN] 46:9c:34:91:7f:12 | RSSI: -66 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 fc 35 82 10 02 2a 04
[SCAN] d9:9f:34:c5:b4:d0 | RSSI: -69 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 03
[SCAN] 71:56:33:da:8d:72 | RSSI: -73 | Name: "" | MFR (10 bytes): 4c 00 10 06 31 1e ce c3 3b 87
[SCAN] 7b:bb:bb:58:a4:11 | RSSI: -87 | Name: "" | MFR (8 bytes): e0 00 00 23 ca 7c d1 9d  | Services: 0xfe9f
[SCAN] 58:7d:8d:b8:a9:f3 | RSSI: -59 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 00 1b 5c 10 02 28 04
[SCAN] 68:b2:01:fb:50:4f | RSSI: -42 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 0f 98 75 79 4c 2c 23
[SCAN] 6a:ef:f4:28:99:54 | RSSI: -65 | Name: "" | MFR (9 bytes): 4c 00 10 05 0e 18 a3 69 82
[SCAN] 4e:04:cd:ca:ba:1e | RSSI: -42 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] c8:c1:7d:90:57:d4 | RSSI: -57 | Name: "Smart Smoke Alarm" | MFR (21 bytes): 4c 00 06 31 00 56 40 c4 0b d5 08 0a 00 25 00 05 02 28 02 8b ec
[SCAN] 5e:1a:a6:4b:d1:70 | RSSI: -79 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 6e 0a 00 00 d0 1b 58 16 08 00 72 23 9a ac b8 2c 0c
[SCAN] de:7a:54:e1:83:d4 | RSSI: -37 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] f0:f5:bd:31:35:66 | RSSI: -80 | Name: "EF-R3BP0410" | MFR (26 bytes): b5 b5 13 52 36 35 35 5a 45 42 34 58 47 42 50 30 34 31 30 46 00 01 00 00 3e 33
[SCAN] e7:83:de:cf:b8:20 | RSSI: -42 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] c4:46:c7:cd:94:01 | RSSI: -79 | Name: ""
[SCAN] 6e:34:3f:16:ff:6c | RSSI: -80 | Name: "" | MFR (9 bytes): 4c 00 10 05 06 18 4c 35 65
[SCAN] 58:d3:49:e1:d1:28 | RSSI: -78 | Name: "" | MFR (9 bytes): 4c 00 10 05 0e 14 1a ec 5d
[SCAN] 62:e9:e8:77:c9:b3 | RSSI: -77 | Name: "" | MFR (10 bytes): 4c 00 10 06 3d 1e 18 8f ba f0
[SCAN] 4a:7d:07:a0:e7:a9 | RSSI: -42 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] fb:28:5a:85:53:95 | RSSI: -71 | Name: "" | MFR (6 bytes): 4c 00 12 02 94 00
[SCAN] 50:b7:50:e3:93:4b | RSSI: -47 | Name: "" | MFR (11 bytes): 4c 00 10 07 3f 1f a0 e1 b0 0d 48
[SCAN] f7:8d:dc:9d:f1:fc | RSSI: -77 | Name: "" | MFR (29 bytes): 4c 00 12 19 00 54 f3 70 73 50 37 3f a8 62 44 d2 e5 7b 81 a9 de 53 58 99 d4 30 24 01 00
[SCAN] 7a:c4:29:36:27:3f | RSSI: -34 | Name: "" | MFR (10 bytes): 4c 00 10 06 0b 1d f8 e4 4f 48
[SCAN] 55:49:5c:cf:48:63 | RSSI: -56 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 49:2b:69:75:0c:6e | RSSI: -58 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 47 87 87 10 02 29 04
[SCAN] 4e:58:41:ac:84:f0 | RSSI: -82 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 a6 94 b5 10 02 2c 04
[SCAN] 18:c2:3c:95:e8:01 | RSSI: -41 | Name: "Aqara-0804-f2f" | MFR (15 bytes): 27 0b 08 10 00 f2 f1 15 f0 80 4a ff 9f 03 c0  | Services: 0xfcb9
[SCAN] fc:17:c7:63:90:d4 | RSSI: -75 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] ec:ac:be:71:a3:61 | RSSI: -75 | Name: "" | MFR (6 bytes): 4c 00 12 02 10 00
[SCAN] 33:0b:2c:20:2a:d4 | RSSI: -76 | Name: "" | MFR (29 bytes): 06 00 01 09 20 22 cc df e2 06 69 54 c1 b0 47 60 05 b0 ef 15 81 37 61 b7 10 85 74 e1 f5
[SCAN] e3:74:61:9c:be:32 | RSSI: -72 | Name: "" | MFR (6 bytes): 4c 00 12 02 14 00
[SCAN] c3:df:bb:ae:dc:8b | RSSI: -68 | Name: "vuart:ktunnel" | MFR (4 bytes): 27 0b 00 00
[SCAN] 46:84:0d:e9:a7:e2 | RSSI: -80 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] 61:5a:c4:e3:61:f1 | RSSI: -69 | Name: "" | MFR (9 bytes): 4c 00 10 05 4b 1c 69 4e 49
[SCAN] 20:57:9e:66:d2:2c | RSSI: -60 | Name: "SKY0766"
[SCAN] 80:6f:b0:23:f4:a0 | RSSI: -76 | Name: "S80e70c8bf41ad7a5C" | MFR (25 bytes): 4c 00 02 15 74 27 8b da b6 44 45 20 8f 0c 72 0e af 05 99 35 00 00 7a 0d c5  | Services: 0x1122
[SCAN] 7e:12:a8:fa:46:54 | RSSI: -80 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 4c bb 82 f7 2d a9 41
[SCAN] 75:e0:56:f4:24:ff | RSSI: -47 | Name: "" | MFR (9 bytes): 4c 00 10 05 2c 18 00 ae 06
[SCAN] 42:3c:bb:5e:8e:16 | RSSI: -67 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9a 0a 00 00 b1 1b 58 16 08 00 ab 8d 17 ec 79 68 4a
[SCAN] d2:d2:ba:e7:21:5d | RSSI: -72 | Name: "S16 9C38 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 58:0b:2d:03:a9:6c | RSSI: -83 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 c3 1b 58 16 08 00 f9 1c c9 65 11 d5 5b
[SCAN] f8:5e:d1:b8:77:37 | RSSI: -70 | Name: "HomeHeatS" | MFR (3 bytes): f1 04 10
[SCAN] 4a:89:e6:b5:cd:9e | RSSI: -89 | Name: "" | MFR (11 bytes): 4c 00 10 07 0b 1f 1c 8f e7 71 48
[SCAN] 35:6f:f3:af:c4:50 | RSSI: -62 | Name: "" | MFR (12 bytes): 4c 00 13 08 5a a9 f1 59 3a 4e 25 00
[SCAN] f0:9d:02:97:34:af | RSSI: -88 | Name: "" | MFR (6 bytes): 4c 00 12 02 54 03
[SCAN] 6e:9b:4d:50:a8:a7 | RSSI: -61 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 49 1b 58 16 08 00 6e 96 1a 95 15 f9 54
[SCAN] 59:6c:a9:9c:d6:a4 | RSSI: -75 | Name: "" | MFR (9 bytes): 4c 00 10 05 00 1c 66 75 3b
[SCAN] f7:31:b7:bd:4f:23 | RSSI: -41 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] 1f:02:e3:9f:09:01 | RSSI: -74 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a ed 67 89 ad 09 fc 00
[SCAN] d2:d2:04:c9:06:cd | RSSI: -73 | Name: "S16 31CF LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 7d:c4:50:79:e9:94 | RSSI: -76 | Name: "" | MFR (25 bytes): 4c 00 02 15 50 76 5c b7 d9 ea 4e 21 99 a4 fa 87 96 13 a4 92 6e 0b bb 69 ce
[SCAN] 38:29:35:f9:21:43 | RSSI: -61 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a c8 5c bd 7b 69 ba 00
[SCAN] 6d:f5:d1:bf:66:17 | RSSI: -53 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 a0 d8 b9 ab a5 5a 95
[SCAN] 43:71:47:54:2e:1f | RSSI: -60 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 a8 0a 00 00 ee 1b 58 16 08 00 3d 86 e6 2f c1 fe e1
[SCAN] 6b:d6:31:fb:23:bc | RSSI: -56 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 68 1b 58 16 08 00 af 51 d2 1f 9b fc 05
[SCAN] fa:6c:ab:f6:42:bb | RSSI: -70 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] e4:88:dc:a8:e8:e2 | RSSI: -76 | Name: "DoorLocker" | MFR (15 bytes): 27 0b 28 08 03 ae a1 15 f2 80 2a 12 fc 22 80  | Services: 0xfcb9
[SCAN] 70:49:c4:53:83:c5 | RSSI: -73 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 05 1b 58 16 08 00 18 1b ca 8c 9b 1c af
[SCAN] d2:d2:3e:21:65:1f | RSSI: -74 | Name: "S16 9C77 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 13:bc:2f:75:5d:21 | RSSI: -83 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a 23 c8 2b 58 e0 63 00
[SCAN] f3:6b:ac:6c:65:0f | RSSI: -72 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] 6b:13:cc:81:c4:b3 | RSSI: -61 | Name: "" | MFR (9 bytes): 4c 00 10 05 13 18 ba 8d d6
[SCAN] 46:9c:34:91:7f:12 | RSSI: -68 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 fc 35 82 10 02 2a 04
[SCAN] 49:ed:e8:2c:42:dd | RSSI: -76 | Name: "" | MFR (28 bytes): 2d 01 02 00 01 10 dc 56 df 5d ed 6b 49 6a aa e2 55 da 99 c9 2d da ff 0d d5 06 25 c5
[SCAN] 8c:f6:81:fe:8a:f6 | RSSI: -76 | Name: "WB237202" | Services: 331a36f5-2459-45ea-9d95-6142f0c4b307
[SCAN] e7:a7:8e:13:94:9d | RSSI: -86 | Name: "S17 695E LE" | MFR (19 bytes): a7 05 06 00 12 00 29 00 c8 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 58:7d:8d:b8:a9:f3 | RSSI: -61 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 00 1b 5c 10 02 28 04
[SCAN] 68:b2:01:fb:50:4f | RSSI: -42 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 0f 98 75 79 4c 2c 23
[SCAN] 63:40:59:46:b8:5b | RSSI: -72 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 63:40:59:46:b8:5b | RSSI: -72 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 6a:e9:98:6a:01:5b | RSSI: -81 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] 67:df:31:de:88:bd | RSSI: -69 | Name: "" | MFR (9 bytes): 4c 00 10 05 27 98 54 60 c4
[SCAN] 4e:04:cd:ca:ba:1e | RSSI: -42 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] f0:f5:bd:31:35:66 | RSSI: -82 | Name: "EF-R3BP0410" | MFR (26 bytes): b5 b5 13 52 36 35 35 5a 45 42 34 58 47 42 50 30 34 31 30 46 00 01 00 00 3e 33
[SCAN] de:de:13:3e:bf:1c | RSSI: -55 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] c4:46:c7:cd:94:01 | RSSI: -79 | Name: "" | MFR (21 bytes): 2d 01 04 00 01 31 05 01 e5 31 e2 32 04 40 d5 00 00 00 00 00 00
[SCAN] 68:27:37:7d:32:8e | RSSI: -86 | Name: "" | MFR (26 bytes): 75 00 42 04 01 80 60 68 27 37 7d 32 8e 6a 27 37 7d 32 8d 01 00 00 00 00 00 00
[SCAN] 6a:ef:f4:28:99:54 | RSSI: -64 | Name: "" | MFR (9 bytes): 4c 00 10 05 0e 18 a3 69 82
[SCAN] 4a:7d:07:a0:e7:a9 | RSSI: -43 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] e3:72:38:30:88:7b | RSSI: -39 | Name: "" | MFR (21 bytes): 4c 00 07 11 06 0b b0 5a 23 a7 49 1c e3 82 2a 74 62 9e f9 79 c4
[SCAN] dd:1e:5f:14:76:8e | RSSI: -70 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] 5d:72:52:92:a5:89 | RSSI: -60 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 7a:c4:29:36:27:3f | RSSI: -78 | Name: "" | MFR (10 bytes): 4c 00 10 06 0b 1d f8 e4 4f 48
[SCAN] 49:2b:69:75:0c:6e | RSSI: -60 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 47 87 87 10 02 29 04
[SCAN] 4e:58:41:ac:84:f0 | RSSI: -71 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 a6 94 b5 10 02 2c 04
[SCAN] 5e:1a:a6:4b:d1:70 | RSSI: -87 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 6e 0a 00 00 d0 1b 58 16 08 00 72 23 9a ac b8 2c 0c
[SCAN] 18:c2:3c:95:e8:01 | RSSI: -42 | Name: "AqaraRecorder" | Services: 0xfcb9
[SCAN] 62:e9:e8:77:c9:b3 | RSSI: -79 | Name: "" | MFR (10 bytes): 4c 00 10 06 3d 1e 18 8f ba f0
[SCAN] 50:b7:50:e3:93:4b | RSSI: -53 | Name: "" | MFR (11 bytes): 4c 00 10 07 3f 1f a0 e1 b0 0d 48
[SCAN] e5:e7:d1:78:30:fc | RSSI: -82 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] 55:49:5c:cf:48:63 | RSSI: -72 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 65:0d:06:a3:58:7e | RSSI: -73 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 33:0b:2c:20:2a:d4 | RSSI: -78 | Name: "" | MFR (29 bytes): 06 00 01 09 20 22 cc df e2 06 69 54 c1 b0 47 60 05 b0 ef 15 81 37 61 b7 10 85 74 e1 f5
[SCAN] de:7a:54:e1:83:d4 | RSSI: -35 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] 20:57:9e:66:d2:2c | RSSI: -60 | Name: "SKY0766"
[SCAN] e7:83:de:cf:b8:20 | RSSI: -57 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] fa:f0:11:6f:24:67 | RSSI: -72 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] 75:e0:56:f4:24:ff | RSSI: -44 | Name: "" | MFR (9 bytes): 4c 00 10 05 2c 18 00 ae 06
[SCAN] 7e:12:a8:fa:46:54 | RSSI: -81 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 4c bb 82 f7 2d a9 41
[SCAN] fb:28:5a:85:53:95 | RSSI: -60 | Name: "" | MFR (6 bytes): 4c 00 12 02 94 00
[SCAN] d2:d2:ba:e7:21:5d | RSSI: -76 | Name: "S16 9C38 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 80:6f:b0:23:f4:a0 | RSSI: -75 | Name: "S80e70c8bf41ad7a5C" | MFR (25 bytes): 4c 00 02 15 74 27 8b da b6 44 45 20 8f 0c 72 0e af 05 99 35 00 00 7a 0d c5  | Services: 0x1122
[SCAN] 71:56:33:da:8d:72 | RSSI: -76 | Name: "" | MFR (10 bytes): 4c 00 10 06 31 1e ce c3 3b 87
[SCAN] f8:5e:d1:b8:77:37 | RSSI: -76 | Name: "HomeHeatS" | MFR (3 bytes): f1 04 10
[SCAN] fc:17:c7:63:90:d4 | RSSI: -76 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] 58:0b:2d:03:a9:6c | RSSI: -72 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 c3 1b 58 16 08 00 f9 1c c9 65 11 d5 5b
[SCAN] d1:0b:4b:f6:6f:3b | RSSI: -72 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] 35:6f:f3:af:c4:50 | RSSI: -60 | Name: "" | MFR (12 bytes): 4c 00 13 08 5a a9 f1 59 3a 4e 25 00
[SCAN] 6e:9b:4d:50:a8:a7 | RSSI: -61 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 49 1b 58 16 08 00 6e 96 1a 95 15 f9 54
[SCAN] 42:3c:bb:5e:8e:16 | RSSI: -66 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9a 0a 00 00 b1 1b 58 16 08 00 ab 8d 17 ec 79 68 4a
[SCAN] 46:84:0d:e9:a7:e2 | RSSI: -80 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] 38:29:35:f9:21:43 | RSSI: -62 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a c8 5c bd 7b 69 ba 00
[SCAN] d2:d2:04:c9:06:cd | RSSI: -70 | Name: "S16 31CF LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 43:71:47:54:2e:1f | RSSI: -59 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 a8 0a 00 00 ee 1b 58 16 08 00 3d 86 e6 2f c1 fe e1
[SCAN] 7d:c4:50:79:e9:94 | RSSI: -72 | Name: "" | MFR (25 bytes): 4c 00 02 15 50 76 5c b7 d9 ea 4e 21 99 a4 fa 87 96 13 a4 92 6e 0b bb 69 ce
[SCAN] 6b:d6:31:fb:23:bc | RSSI: -60 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 68 1b 58 16 08 00 af 51 d2 1f 9b fc 05
[SCAN] f7:31:b7:bd:4f:23 | RSSI: -44 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] 1f:02:e3:9f:09:01 | RSSI: -68 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a ed 67 89 ad 09 fc 00
[SCAN] 70:49:c4:53:83:c5 | RSSI: -77 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 05 1b 58 16 08 00 18 1b ca 8c 9b 1c af
[SCAN] 13:bc:2f:75:5d:21 | RSSI: -72 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a 23 c8 2b 58 e0 63 00
[SCAN] 60:7d:fa:21:65:9d | RSSI: -71 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 9d 46 dc 83 ee 3f 1d
[SCAN] fa:6c:ab:f6:42:bb | RSSI: -71 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] 8c:f6:81:fe:8a:f6 | RSSI: -81 | Name: "WB237202" | Services: 331a36f5-2459-45ea-9d95-6142f0c4b307
[SCAN] 49:ed:e8:2c:42:dd | RSSI: -66 | Name: "" | MFR (28 bytes): 2d 01 02 00 01 10 dc 56 df 5d ed 6b 49 6a aa e2 55 da 99 c9 2d da ff 0d d5 06 25 c5
[SCAN] 46:9c:34:91:7f:12 | RSSI: -67 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 fc 35 82 10 02 2a 04
[SCAN] d2:d2:3e:21:65:1f | RSSI: -78 | Name: "S16 9C77 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 59:6c:a9:9c:d6:a4 | RSSI: -79 | Name: "" | MFR (9 bytes): 4c 00 10 05 00 1c 66 75 3b
[SCAN] 58:7d:8d:b8:a9:f3 | RSSI: -68 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 00 1b 5c 10 02 28 04
[SCAN] 68:b2:01:fb:50:4f | RSSI: -41 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 0f 98 75 79 4c 2c 23
[SCAN] 1c:86:9a:df:a5:cf | RSSI: -86 | Name: "The Frame 85" | MFR (26 bytes): 75 00 42 04 01 80 7e 1c 86 9a df a5 cf 1e 86 9a df a5 ce 01 a8 00 00 00 00 00
[SCAN] 61:5a:c4:e3:61:f1 | RSSI: -74 | Name: "" | MFR (9 bytes): 4c 00 10 05 4b 1c 69 4e 49
[SCAN] 63:40:59:46:b8:5b | RSSI: -74 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 63:40:59:46:b8:5b | RSSI: -74 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] e7:a7:8e:13:94:9d | RSSI: -88 | Name: "S17 695E LE" | MFR (19 bytes): a7 05 06 00 12 00 29 00 c8 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] f0:f5:bd:31:35:66 | RSSI: -81 | Name: "EF-R3BP0410" | MFR (26 bytes): b5 b5 13 52 36 35 35 5a 45 42 34 58 47 42 50 30 34 31 30 46 00 01 00 00 3e 33
[SCAN] c4:46:c7:cd:94:01 | RSSI: -67 | Name: "LE_WH-1000XM4" | Services: 0xfe03
[SCAN] d3:3b:04:bd:11:4d | RSSI: -75 | Name: "" | MFR (6 bytes): 4c 00 12 02 25 03
[SCAN] e9:b3:00:28:c5:5f | RSSI: -84 | Name: "" | MFR (6 bytes): 4c 00 12 02 2c 03
[SCAN] 6b:13:cc:81:c4:b3 | RSSI: -71 | Name: "" | MFR (9 bytes): 4c 00 10 05 13 18 ba 8d d6
[SCAN] e3:72:38:30:88:7b | RSSI: -38 | Name: "" | MFR (21 bytes): 4c 00 07 11 06 0b b0 5a 23 a7 49 1c e3 82 2a 74 62 9e f9 79 c4
[SCAN] 67:df:31:de:88:bd | RSSI: -78 | Name: "" | MFR (9 bytes): 4c 00 10 05 27 98 54 60 c4
[SCAN] 4a:7d:07:a0:e7:a9 | RSSI: -65 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] 7b:bb:bb:58:a4:11 | RSSI: -85 | Name: "" | MFR (8 bytes): e0 00 00 23 ca 7c d1 9d  | Services: 0xfe9f
[SCAN] e7:da:d0:88:66:c1 | RSSI: -49 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 03
[SCAN] 6a:e9:98:6a:01:5b | RSSI: -82 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] d9:9f:34:c5:b4:d0 | RSSI: -78 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 03
[SCAN] 49:2b:69:75:0c:6e | RSSI: -60 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 47 87 87 10 02 29 04
[SCAN] 6e:34:3f:16:ff:6c | RSSI: -84 | Name: "" | MFR (9 bytes): 4c 00 10 05 06 18 4c 35 65
[SCAN] 4e:58:41:ac:84:f0 | RSSI: -73 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 a6 94 b5 10 02 2c 04
[SCAN] 4e:04:cd:ca:ba:1e | RSSI: -41 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] 5e:1a:a6:4b:d1:70 | RSSI: -80 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 6e 0a 00 00 d0 1b 58 16 08 00 72 23 9a ac b8 2c 0c
[SCAN] 62:e9:e8:77:c9:b3 | RSSI: -76 | Name: "" | MFR (10 bytes): 4c 00 10 06 3d 1e 18 8f ba f0
[SCAN] 5d:72:52:92:a5:89 | RSSI: -69 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 50:b7:50:e3:93:4b | RSSI: -54 | Name: "" | MFR (11 bytes): 4c 00 10 07 3f 1f a0 e1 b0 0d 48
[SCAN] 7a:c4:29:36:27:3f | RSSI: -35 | Name: "" | MFR (10 bytes): 4c 00 10 06 0b 1d f8 e4 4f 48
[SCAN] 55:49:5c:cf:48:63 | RSSI: -55 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] c8:c1:7d:90:57:d4 | RSSI: -49 | Name: "Smart Smoke Alarm" | MFR (21 bytes): 4c 00 06 31 00 56 40 c4 0b d5 08 0a 00 25 00 05 02 28 02 8b ec
[SCAN] 18:c2:3c:95:e8:01 | RSSI: -40 | Name: "Aqara-0804-f2f" | MFR (15 bytes): 27 0b 08 10 00 f2 f1 15 f0 80 4a ff 9f 03 c0  | Services: 0xfcb9
[SCAN] 20:57:9e:66:d2:2c | RSSI: -58 | Name: "SKY0766"
[SCAN] 7e:12:a8:fa:46:54 | RSSI: -81 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 4c bb 82 f7 2d a9 41
[SCAN] e4:88:dc:a8:e8:e2 | RSSI: -85 | Name: "DoorLocker" | MFR (15 bytes): 27 0b 28 08 03 ae a1 15 f2 80 2a 12 fc 22 80  | Services: 0xfcb9
[SCAN] d2:d2:ba:e7:21:5d | RSSI: -74 | Name: "S16 9C38 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 6a:ef:f4:28:99:54 | RSSI: -65 | Name: "" | MFR (9 bytes): 4c 00 10 05 0e 18 a3 69 82
[SCAN] 65:0d:06:a3:58:7e | RSSI: -79 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 75:e0:56:f4:24:ff | RSSI: -48 | Name: "" | MFR (9 bytes): 4c 00 10 05 2c 18 00 ae 06
[SCAN] f8:5e:d1:b8:77:37 | RSSI: -75 | Name: "HomeHeatS" | MFR (3 bytes): f1 04 10
[SCAN] fa:f0:11:6f:24:67 | RSSI: -72 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] 71:56:33:da:8d:72 | RSSI: -69 | Name: "" | MFR (10 bytes): 4c 00 10 06 31 1e ce c3 3b 87
[SCAN] fb:28:5a:85:53:95 | RSSI: -68 | Name: "" | MFR (6 bytes): 4c 00 12 02 94 00
[SCAN] 33:0b:2c:20:2a:d4 | RSSI: -75 | Name: "" | MFR (29 bytes): 06 00 01 09 20 22 cc df e2 06 69 54 c1 b0 47 60 05 b0 ef 15 81 37 61 b7 10 85 74 e1 f5
[SCAN] 4a:89:e6:b5:cd:9e | RSSI: -88 | Name: "" | MFR (11 bytes): 4c 00 10 07 0b 1f 1c 8f e7 71 48
[SCAN] 42:3c:bb:5e:8e:16 | RSSI: -67 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9a 0a 00 00 b1 1b 58 16 08 00 ab 8d 17 ec 79 68 4a
[SCAN] 38:29:35:f9:21:43 | RSSI: -59 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a c8 5c bd 7b 69 ba 00
[SCAN] d0:f0:c4:bc:aa:9f | RSSI: -66 | Name: "" | MFR (6 bytes): 4c 00 12 02 14 03
[SCAN] 58:0b:2d:03:a9:6c | RSSI: -75 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 c3 1b 58 16 08 00 f9 1c c9 65 11 d5 5b
[SCAN] fc:17:c7:63:90:d4 | RSSI: -69 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] ec:ac:be:71:a3:61 | RSSI: -71 | Name: "" | MFR (6 bytes): 4c 00 12 02 10 00
[SCAN] 35:6f:f3:af:c4:50 | RSSI: -60 | Name: "" | MFR (12 bytes): 4c 00 13 08 5a a9 f1 59 3a 4e 25 00
[SCAN] 6e:9b:4d:50:a8:a7 | RSSI: -62 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 49 1b 58 16 08 00 6e 96 1a 95 15 f9 54
[SCAN] e8:21:cf:6e:c4:7c | RSSI: -89 | Name: "" | MFR (6 bytes): 4c 00 12 02 3c 02
[SCAN] 1f:02:e3:9f:09:01 | RSSI: -62 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a ed 67 89 ad 09 fc 00
[SCAN] 13:bc:2f:75:5d:21 | RSSI: -73 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a 23 c8 2b 58 e0 63 00
[SCAN] 8c:f6:81:fe:8a:f6 | RSSI: -70 | Name: "WB237202" | Services: 331a36f5-2459-45ea-9d95-6142f0c4b307
[SCAN] d1:0b:4b:f6:6f:3b | RSSI: -77 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] 43:71:47:54:2e:1f | RSSI: -58 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 a8 0a 00 00 ee 1b 58 16 08 00 3d 86 e6 2f c1 fe e1
[SCAN] 6b:d6:31:fb:23:bc | RSSI: -60 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 68 1b 58 16 08 00 af 51 d2 1f 9b fc 05
[SCAN] 46:84:0d:e9:a7:e2 | RSSI: -80 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] 58:7d:8d:b8:a9:f3 | RSSI: -60 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 00 1b 5c 10 02 28 04
[SCAN] 46:9c:34:91:7f:12 | RSSI: -68 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 fc 35 82 10 02 2a 04
[SCAN] e2:50:19:b5:7e:ca | RSSI: -87 | Name: "" | MFR (29 bytes): 4c 00 12 19 10 38 39 7a c1 79 26 a7 d0 23 63 a2 2a fe 08 ce f9 bc 14 da fa 65 f9 00 a0
[SCAN] 59:6c:a9:9c:d6:a4 | RSSI: -70 | Name: "" | MFR (9 bytes): 4c 00 10 05 00 1c 66 75 3b
[SCAN] 49:ed:e8:2c:42:dd | RSSI: -77 | Name: "" | MFR (28 bytes): 2d 01 02 00 01 10 dc 56 df 5d ed 6b 49 6a aa e2 55 da 99 c9 2d da ff 0d d5 06 25 c5
[SCAN] 70:49:c4:53:83:c5 | RSSI: -74 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 05 1b 58 16 08 00 18 1b ca 8c 9b 1c af
[SCAN] 80:6f:b0:23:f4:a0 | RSSI: -75 | Name: "S80e70c8bf41ad7a5C" | MFR (25 bytes): 4c 00 02 15 74 27 8b da b6 44 45 20 8f 0c 72 0e af 05 99 35 00 00 7a 0d c5  | Services: 0x1122
[SCAN] 63:40:59:46:b8:5b | RSSI: -74 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 63:40:59:46:b8:5b | RSSI: -74 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 68:b2:01:fb:50:4f | RSSI: -42 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 0f 98 75 79 4c 2c 23
[SCAN] d2:d2:3e:21:65:1f | RSSI: -75 | Name: "S16 9C77 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 7d:c4:50:79:e9:94 | RSSI: -65 | Name: "" | MFR (25 bytes): 4c 00 02 15 50 76 5c b7 d9 ea 4e 21 99 a4 fa 87 96 13 a4 92 6e 0b bb 69 ce
[SCAN] f0:f5:bd:31:35:66 | RSSI: -83 | Name: "EF-R3BP0410" | MFR (26 bytes): b5 b5 13 52 36 35 35 5a 45 42 34 58 47 42 50 30 34 31 30 46 00 01 00 00 3e 33
[SCAN] f7:31:b7:bd:4f:23 | RSSI: -41 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] e7:a7:8e:13:94:9d | RSSI: -85 | Name: "S17 695E LE" | MFR (19 bytes): a7 05 06 00 12 00 29 00 c8 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] c4:46:c7:cd:94:01 | RSSI: -64 | Name: "LE_WH-1000XM4" | Services: 0xfe03
[SCAN] 6b:13:cc:81:c4:b3 | RSSI: -60 | Name: "" | MFR (9 bytes): 4c 00 10 05 13 18 ba 8d d6
[SCAN] fa:6c:ab:f6:42:bb | RSSI: -72 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] 60:7d:fa:21:65:9d | RSSI: -72 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 9d 46 dc 83 ee 3f 1d
[SCAN] 67:df:31:de:88:bd | RSSI: -73 | Name: "" | MFR (9 bytes): 4c 00 10 05 27 98 54 60 c4
[SCAN] 4a:7d:07:a0:e7:a9 | RSSI: -78 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] 49:2b:69:75:0c:6e | RSSI: -60 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 47 87 87 10 02 29 04
[SCAN] f3:6b:ac:6c:65:0f | RSSI: -79 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] fb:40:f0:f2:c8:58 | RSSI: -88 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] d2:d2:04:c9:06:cd | RSSI: -72 | Name: "S16 31CF LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 5e:1a:a6:4b:d1:70 | RSSI: -79 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 6e 0a 00 00 d0 1b 58 16 08 00 72 23 9a ac b8 2c 0c
[SCAN] 4e:04:cd:ca:ba:1e | RSSI: -67 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] 4e:58:41:ac:84:f0 | RSSI: -76 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 a6 94 b5 10 02 2c 04
[SCAN] 68:27:37:7d:32:8e | RSSI: -88 | Name: "" | MFR (26 bytes): 75 00 42 04 01 80 60 68 27 37 7d 32 8e 6a 27 37 7d 32 8d 01 00 00 00 00 00 00
[SCAN] c3:df:bb:ae:dc:8b | RSSI: -69 | Name: "vuart:ktunnel" | MFR (4 bytes): 27 0b 00 00
[SCAN] 6e:2f:ce:04:83:62 | RSSI: -89 | Name: "" | MFR (11 bytes): 4c 00 10 07 3a 1f 7f f3 fa 4b 18
[SCAN] de:de:13:3e:bf:1c | RSSI: -59 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] d3:3b:04:bd:11:4d | RSSI: -84 | Name: "" | MFR (6 bytes): 4c 00 12 02 25 03
[SCAN] 62:e9:e8:77:c9:b3 | RSSI: -75 | Name: "" | MFR (10 bytes): 4c 00 10 06 3d 1e 18 8f ba f0
[SCAN] 7b:bb:bb:58:a4:11 | RSSI: -84 | Name: "" | MFR (8 bytes): e0 00 00 23 ca 7c d1 9d  | Services: 0xfe9f
[SCAN] 50:b7:50:e3:93:4b | RSSI: -50 | Name: "" | MFR (11 bytes): 4c 00 10 07 3f 1f a0 e1 b0 0d 48
[SCAN] 20:57:9e:66:d2:2c | RSSI: -60 | Name: "SKY0766"
[SCAN] 7a:c4:29:36:27:3f | RSSI: -50 | Name: "" | MFR (10 bytes): 4c 00 10 06 0b 1d f8 e4 4f 48
[SCAN] 55:49:5c:cf:48:63 | RSSI: -79 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 18:c2:3c:95:e8:01 | RSSI: -40 | Name: "AqaraRecorder" | Services: 0xfcb9
[SCAN] e3:72:38:30:88:7b | RSSI: -46 | Name: "" | MFR (21 bytes): 4c 00 07 11 06 0b b0 5a 23 a7 49 1c e3 82 2a 74 62 9e f9 79 c4
[SCAN] d2:d2:ba:e7:21:5d | RSSI: -76 | Name: "S16 9C38 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 7e:12:a8:fa:46:54 | RSSI: -81 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 4c bb 82 f7 2d a9 41
[SCAN] 33:0b:2c:20:2a:d4 | RSSI: -73 | Name: "" | MFR (29 bytes): 06 00 01 09 20 22 cc df e2 06 69 54 c1 b0 47 60 05 b0 ef 15 81 37 61 b7 10 85 74 e1 f5
[SCAN] dd:1e:5f:14:76:8e | RSSI: -85 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] 4a:89:e6:b5:cd:9e | RSSI: -85 | Name: "" | MFR (11 bytes): 4c 00 10 07 0b 1f 1c 8f e7 71 48
[SCAN] 6a:ef:f4:28:99:54 | RSSI: -65 | Name: "" | MFR (9 bytes): 4c 00 10 05 0e 18 a3 69 82
[SCAN] 65:0d:06:a3:58:7e | RSSI: -72 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] e7:da:d0:88:66:c1 | RSSI: -47 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 03
[SCAN] d9:9f:34:c5:b4:d0 | RSSI: -67 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 03
[SCAN] 58:0b:2d:03:a9:6c | RSSI: -77 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 c3 1b 58 16 08 00 f9 1c c9 65 11 d5 5b
[SCAN] 35:6f:f3:af:c4:50 | RSSI: -62 | Name: "" | MFR (12 bytes): 4c 00 13 08 5a a9 f1 59 3a 4e 25 00
[SCAN] e5:e7:d1:78:30:fc | RSSI: -82 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 00
[SCAN] 6a:e9:98:6a:01:5b | RSSI: -79 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] 6e:9b:4d:50:a8:a7 | RSSI: -66 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9e 0a 00 00 49 1b 58 16 08 00 6e 96 1a 95 15 f9 54
[SCAN] e4:88:dc:a8:e8:e2 | RSSI: -69 | Name: "DoorLocker" | MFR (15 bytes): 27 0b 28 08 03 ae a1 15 f2 80 2a 12 fc 22 80  | Services: 0xfcb9
[SCAN] 8c:f6:81:fe:8a:f6 | RSSI: -70 | Name: "WB237202" | Services: 331a36f5-2459-45ea-9d95-6142f0c4b307
[SCAN] 42:3c:bb:5e:8e:16 | RSSI: -65 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 9a 0a 00 00 b1 1b 58 16 08 00 ab 8d 17 ec 79 68 4a
[SCAN] 13:bc:2f:75:5d:21 | RSSI: -73 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a 23 c8 2b 58 e0 63 00
[SCAN] 38:29:35:f9:21:43 | RSSI: -66 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a c8 5c bd 7b 69 ba 00
[SCAN] 43:71:47:54:2e:1f | RSSI: -60 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 a8 0a 00 00 ee 1b 58 16 08 00 3d 86 e6 2f c1 fe e1
[SCAN] 58:7d:8d:b8:a9:f3 | RSSI: -60 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 00 1b 5c 10 02 28 04
[SCAN] 46:9c:34:91:7f:12 | RSSI: -68 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 fc 35 82 10 02 2a 04
[SCAN] de:7a:54:e1:83:d4 | RSSI: -38 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] e7:83:de:cf:b8:20 | RSSI: -44 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] 1c:86:9a:df:a5:cf | RSSI: -87 | Name: "The Frame 85" | MFR (26 bytes): 75 00 42 04 01 80 7e 1c 86 9a df a5 cf 1e 86 9a df a5 ce 01 a8 00 00 00 00 00
[SCAN] f8:5e:d1:b8:77:37 | RSSI: -86 | Name: "HomeHeatS" | MFR (3 bytes): f1 04 10
[SCAN] 1f:02:e3:9f:09:01 | RSSI: -84 | Name: "" | MFR (12 bytes): 4c 00 13 08 4a ed 67 89 ad 09 fc 00
[SCAN] fa:f0:11:6f:24:67 | RSSI: -73 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] 70:49:c4:53:83:c5 | RSSI: -72 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 05 1b 58 16 08 00 18 1b ca 8c 9b 1c af
[SCAN] 6b:d6:31:fb:23:bc | RSSI: -57 | Name: "" | MFR (22 bytes): 4c 00 09 08 13 02 0a 00 00 68 1b 58 16 08 00 af 51 d2 1f 9b fc 05
[SCAN] 5d:72:52:92:a5:89 | RSSI: -56 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 7d:c4:50:79:e9:94 | RSSI: -66 | Name: "" | MFR (25 bytes): 4c 00 02 15 50 76 5c b7 d9 ea 4e 21 99 a4 fa 87 96 13 a4 92 6e 0b bb 69 ce
[SCAN] 49:ed:e8:2c:42:dd | RSSI: -66 | Name: "" | MFR (28 bytes): 2d 01 02 00 01 10 dc 56 df 5d ed 6b 49 6a aa e2 55 da 99 c9 2d da ff 0d d5 06 25 c5
[SCAN] d2:d2:3e:21:65:1f | RSSI: -75 | Name: "S16 9C77 LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] d0:f0:c4:bc:aa:9f | RSSI: -77 | Name: "" | MFR (6 bytes): 4c 00 12 02 14 03
[SCAN] 63:40:59:46:b8:5b | RSSI: -72 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 63:40:59:46:b8:5b | RSSI: -72 | Name: "" | MFR (10 bytes): 4c 00 10 06 41 1d a4 bb c2 78
[SCAN] 68:b2:01:fb:50:4f | RSSI: -44 | Name: "" | MFR (12 bytes): 4c 00 16 08 00 0f 98 75 79 4c 2c 23
[SCAN] c4:46:c7:cd:94:01 | RSSI: -67 | Name: "" | MFR (21 bytes): 2d 01 04 00 01 31 05 01 e5 31 e2 32 04 40 d5 00 00 00 00 00 00
[SCAN] 67:df:31:de:88:bd | RSSI: -73 | Name: "" | MFR (9 bytes): 4c 00 10 05 27 98 54 60 c4
[SCAN] fc:17:c7:63:90:d4 | RSSI: -73 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] cf:e5:31:d9:66:d1 | RSSI: -74 | Name: "" | MFR (25 bytes): 4c 00 12 02 64 00 07 11 06 23 d4 17 5a 60 b9 d3 9e 12 31 07 8b 14 ed a7 34
[SCAN] 4a:7d:07:a0:e7:a9 | RSSI: -44 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] d2:d2:04:c9:06:cd | RSSI: -72 | Name: "S16 31CF LE" | MFR (19 bytes): a7 05 06 00 12 10 2a 00 ca 00 00 10 00 00 00 00 00 00 00  | Services: 0xfe07
[SCAN] 49:2b:69:75:0c:6e | RSSI: -60 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 47 87 87 10 02 29 04
[SCAN] 6e:34:3f:16:ff:6c | RSSI: -83 | Name: "" | MFR (9 bytes): 4c 00 10 05 06 18 4c 35 65
[SCAN] 80:6f:b0:23:f4:a0 | RSSI: -76 | Name: "S80e70c8bf41ad7a5C" | MFR (25 bytes): 4c 00 02 15 74 27 8b da b6 44 45 20 8f 0c 72 0e af 05 99 35 00 00 7a 0d c5  | Services: 0x1122
[SCAN] 46:84:0d:e9:a7:e2 | RSSI: -82 | Name: "" | MFR (10 bytes): 4c 00 10 06 42 1d fe b7 c6 08
[SCAN] f0:f5:bd:31:35:66 | RSSI: -80 | Name: "EF-R3BP0410" | MFR (26 bytes): b5 b5 13 52 36 35 35 5a 45 42 34 58 47 42 50 30 34 31 30 46 00 01 00 00 3e 33
[SCAN] 4e:58:41:ac:84:f0 | RSSI: -76 | Name: "" | MFR (13 bytes): 4c 00 0f 05 98 00 a6 94 b5 10 02 2c 04
[SCAN] 61:5a:c4:e3:61:f1 | RSSI: -80 | Name: "" | MFR (9 bytes): 4c 00 10 05 4b 1c 69 4e 49
[SCAN] 62:e9:e8:77:c9:b3 | RSSI: -79 | Name: "" | MFR (10 bytes): 4c 00 10 06 3d 1e 18 8f ba f0
[SCAN] d1:0b:4b:f6:6f:3b | RSSI: -79 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 01
[SCAN] 50:b7:50:e3:93:4b | RSSI: -48 | Name: "" | MFR (11 bytes): 4c 00 10 07 3f 1f a0 e1 b0 0d 48
[SCAN] c3:df:bb:ae:dc:8b | RSSI: -72 | Name: "vuart:ktunnel" | MFR (4 bytes): 27 0b 00 00
[SCAN] 68:27:37:7d:32:8e | RSSI: -90 | Name: "" | MFR (26 bytes): 75 00 42 04 01 80 60 68 27 37 7d 32 8e 6a 27 37 7d 32 8d 01 00 00 00 00 00 00
[SCAN] 7a:c4:29:36:27:3f | RSSI: -36 | Name: "" | MFR (10 bytes): 4c 00 10 06 0b 1d f8 e4 4f 48
[SCAN] 55:49:5c:cf:48:63 | RSSI: -55 | Name: "" | MFR (10 bytes): 4c 00 10 06 0c 19 64 22 26 78
[SCAN] 20:57:9e:66:d2:2c | RSSI: -61 | Name: "SKY0766"
[SCAN] f0:9d:02:97:34:af | RSSI: -87 | Name: "" | MFR (6 bytes): 4c 00 12 02 54 03
[SCAN] f7:31:b7:bd:4f:23 | RSSI: -41 | Name: "" | MFR (6 bytes): 4c 00 12 02 00 02
[SCAN] 4e:04:cd:ca:ba:1e | RSSI: -41 | Name: "" | MFR (26 bytes): 4c 00 0c 0e 08 a2 15 64 e7 6c 65 3e 3d 0c 27 02 31 f1 10 06 4c 1d 69 de 5a 28
[SCAN] 54:cc:a4:40:77:45 | RSSI: -90 | Name: "" | MFR (10 bytes): 4c 00 10 06 45 1d 36 7a de e8
[SCAN] 18:c2:3c:95:e8:01 | RSSI: -41 | Name: "AqaraRecorder" | Services: 0xfcb9
[SCAN] 33:0b:2c:20:2a:d4 | RSSI: -76 | Name: "" | MFR (29 bytes): 06 00 01 09 20 22 cc df e2 06 69 54 c1 b0 47 60 05 b0 ef 15 81 37 61 b7 10 85 74 e1 f5
[SCAN] Scan ended. Found 94 devices. Reason: 0
[ERR] Not connected
```
