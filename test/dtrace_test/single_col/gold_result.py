#! /usr/bin/env python3
import sys

if __name__ == '__main__':
  print("\ntracing started ...\n")
  tss = 4222470910
  print(f"begin ts: {tss:d}")
  htss = 18135374470983830270
  print(f"begin host ts = {htss:d}")
  def factorial(n):
      """Return the factorial of n."""
      if n < 0:
          return None
      result = 1
      for i in range(2, n + 1):
          result *= i
      return result
  print(f"write 0xdeadbeef starting memtile offset 0x100010 and next 4 memory locations")
  print(f"write 0xabcdabcd to handshake offset 0x20")
  val = 4222470910
  print(f"read handshake offset 0x20: {hex(val)}")
  ts1 = 4222470910
  operation = "UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0"
  print("opcode: {}".format(operation))
  print("ucdma write ts: {:d}".format(ts1))
  ts1 = 4222470910
  operation = "UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1"
  print("opcode: {}".format(operation))
  print("ucdma write ts: {:d}".format(ts1))
  ts2 = 4222470910
  print(f"mask write ts: {ts2:d}")
  ts2 = 4222470910
  print(f"mask write ts: {ts2:d}")
  ts2 = 4222470910
  print(f"mask write ts: {ts2:d}")
  ts2 = 4222470910
  print(f"mask write ts: {ts2:d}")
  ts2 = 4222470910
  print(f"mask write ts: {ts2:d}")
  ts2 = 4222470910
  print(f"mask write ts: {ts2:d}")
  print(f"write 0xabcdabcd to memtile 0_1 offset 0")
  print(f"mask write 0x12345678 to memtile 0_1 offset 0, mask value 0x0000FFFF")
  ts3 = 18135374470983830270
  operation = "UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "APPLY_OFFSET_57	 @DMAWRITE_data_0, 1, 2"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "APPLY_OFFSET_57	 @DMAWRITE_data_2, 1, 0"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "MASK_POLL_32	 0x61d0604, 0x1, 0x1"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "MASK_POLL_32	 0x1d0604, 0x1, 0x1"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "MASK_POLL_32	 0x21d0604, 0x1, 0x1"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "APPLY_OFFSET_57	 @DMAWRITE_data_8, 1, 1"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5"
  print("opcode: {operation} timestamp: {ts3:d}")
  ts3 = 18135374470983830270
  operation = "MASK_POLL_32	 0x400c604, 0x1, 0x1"
  print("opcode: {operation} timestamp: {ts3:d}")
  v = 4222470910
  print(f"read from memtile 0_1 offset 0: {hex(v)}")
  ts_start_job = 4222470910
  axi_mm_offset = 4222470910
  print(f"start job ts: {ts_start_job:d} axi_mm_offset: 0x{axi_mm_offset:x}")
  cnt = 0
  operation = "LOAD_PDI 0, @pdi"
  print("opcode: {} count: {}".format(operation, cnt))
  ts_end_job = 18135374470983830270
  print("end job ts: {ts_end_job:d}")
  v = 4222470910
  print(f"read from memtile 1_1 offset 0: {hex(v)}")
  ts4 = [18135374470983830270, 18135374470983830270, 18135374470983830270, 18135374470983830270, 18135374470983830270]
  probename = "tracepoint:uc0:id4"
  print(f"{probename}: page load timestamps: {ts4}")
  ts10 = [18135374470983830270, 18135374470983830270, 18135374470983830270, 18135374470983830270, 18135374470983830270]
  probename = "tracepoint:uc0:id4"
  print(f"{probename}: page load host timestamps: {ts10}")
  ts4 = [18135374470983830270, 18135374470983830270, 18135374470983830270, 18135374470983830270, 18135374470983830270]
  probename = "tracepoint:uc0:id5"
  print(f"{probename}: page load timestamps: {ts4}")
  ts10 = [18135374470983830270, 18135374470983830270, 18135374470983830270, 18135374470983830270, 18135374470983830270]
  probename = "tracepoint:uc0:id5"
  print(f"{probename}: page load host timestamps: {ts10}")
  ts5 = [4222470910, 4222470910, 4222470910, 4222470910, 4222470910]
  probename = "tracepoint:uc0:id6"
  print(f"{probename}: page execute start timestamps: {ts5}")
  ts6 = [4222470910, 4222470910, 4222470910, 4222470910, 4222470910]
  probename = "tracepoint:uc0:id7"
  print(f"{probename}: page execute done timestamps: {ts6}")
  ts7 = 18135374470983830270
  probename = "tracepoint:uc0:id12"
  print("{}: after control code running ts4: {:d}".format(probename, ts7))
  ts8 = 18135374470983830270
  probename = "tracepoint:uc0:id14"
  print("{}: before hsa cmd completion ts5: {:d}".format(probename, ts8))
  buf2 = "[0xfbadcafe, 0xfbadcafe, 0xfbadcafe, 0xfbadcafe]"
  print(f"read mem = {buf2}")
  print(f"buf3 result = {buf3}")
  time_taken = [ts6[i] - ts5[i] for i in range(len(ts5))]
  max_time = max(time_taken)
  max_time_page = time_taken.index(max_time)
  print("page time taken:", time_taken)
  print(f"page {max_time_page} took the most time: {max_time}")
  def main():
      num = 5
      print(f"[PYTHON] Factorial of {num} is {factorial(num)}")
  main()
  time = ts_end_job - ts_start_job
  print(f"total time: {time:d}")
  print("\ntracing ended !!\n")
  sys.exit(0)
