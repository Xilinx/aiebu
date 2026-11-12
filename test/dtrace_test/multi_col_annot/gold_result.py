#! /usr/bin/env python3
import sys

if __name__ == '__main__':
  print("\ntracing started\n")
  operation = "start_job 0"
  print("uc0 op: {}".format(operation))
  operation = "uc_dma_write_des_sync @label0"
  print("uc0 annot0: {}".format(operation))
  operation = "end_job"
  print("uc0 annot1: {}".format(operation))
  print("\ntracing ended\n")
  operation = "start_job 1"
  print("uc2 op: {}".format(operation))
  operation = "uc_dma_write_des_sync @label1"
  print("uc2 annot0: {}".format(operation))
  operation = "end_job"
  print("uc2 annot1: {}".format(operation))
  sys.exit(0)
