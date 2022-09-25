//От F1 по Patch към F2
//
//lseek(f,s.off*sezeof(..), SEEK_SET)
//read(f,&a, sezeof(a))
//if ( a != s.b1)
//   err
//lseek
//write
//
//
//
//uint32_t magic // 0xEFBEADDE
//uint8_t header_version // 0x01
//uint8_t data_version // 0x00 0x01
//16 count
//32 res1
//32 res2
//
//Проверка дали е 16 байта файла
//четем хедъра
//проверяваме дали е този който трябва
//черем
//
//
//TAQ E GOLQM KUR
//
