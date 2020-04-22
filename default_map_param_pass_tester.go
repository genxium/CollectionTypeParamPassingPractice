package main

import (
  "fmt"
)

func func1(param map[int]int) {
  /*
   * That "&param" is NOT EQUAL TO (&tmp).
   */
  fmt.Printf("func1, &param == %p\n", &param);
  param[0] = 9999;
}

func func2(pParam *map[int]int) {
  /*
   * Now "pParam" is EQUAL TO (&tmp).
   */
  fmt.Printf("func2, pParam == %p\n", pParam);
  (*pParam)[0] = 9999;
}

func main() {
  var tmp map[int]int;
  tmp = make(map[int]int, 0);
  tmp[0] = 0;
  tmp[1] = 1;
  tmp[2] = 2;
  tmp[3] = 3;
  fmt.Printf("&tmp == %p\n", &tmp);
  func1(tmp);
  fmt.Printf("After func1\n");
  for k, v := range tmp {
    fmt.Printf("tmp[%d] == %d\n", k, v);
  }
  fmt.Printf("\n");

  func2(&tmp);
  fmt.Printf("After func2\n");
  for k, v := range tmp {
    fmt.Printf("tmp[%d] == %d\n", k, v);
  }
  fmt.Printf("\n");
}
