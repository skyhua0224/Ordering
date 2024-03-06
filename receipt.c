FILE *receiptFILE = fopen("receipt.txt", "w");
      if (*receiptFILE != NULL) {
        fputs("             SkyHua Virtual           \n", receiptFILE);
        fputs(" 菜品            价格         数量        小计\n",
              receiptFILE);
               for (int i = 0; i < orderCount; i++) {
          fputs(" %s            %.2f         %.2f        %,2f\n",
                dishes[i].name, dishes[i].price, dishes[i].orderCount,
                dishes[i].price * dishes[i].orderCount, receiptFILE);
          dishes[i].totalprice =
              dishes[i].price * dishes[i].orderCount + dishes[i].totalprice;
        }
        for (int i = 0; i < orderCount; i++)
        fputs("合计金额 ：              %。2f元\n",  dishes[i].totalprice);
        fputs("实收：   %。2f元\n",(double)((int)totalAmount);
        fputs("找零：   %。2f元\n",totalAmount-(double)(int)totalAmount,receiptFILE);
        fputs("              请收好小票，钱物及时核对",receiptFILE);
        fclose(receiptFILE);