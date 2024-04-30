#include "splashkit.h"

using std::to_string;

typedef struct
{
  int     field1;
  double  field2;
  string  field3;
} my_struct;

typedef enum
{
  OPT_1,
  OPT_2
} my_enum;

typedef union
{
  int         int_val;
  long long   long_val;
  double      dbl_val;
} my_number;

int main()
{
  // Declare variables, strucutres, enums, and unions.
  my_struct var1;
  my_struct var2 = {1, 3.1415, "Fred"};
  my_struct var3 = var2;

  my_enum var4 = OPT_1;

  my_number var5 = { .int_val = 98  };

  // Play with record/structures
  var1.field1 = 2;
  var1.field2 = 6.5;
  var1.field3 = "Wilma";

  write_line( "var1 " +
              to_string(var1.field1) + " " +
              to_string(var1.field2) + " " +
              var1.field3 );

  write_line( "var2 " +
              to_string(var2.field1) + " " +
              to_string(var2.field2) + " " +
              var2.field3 );


  // Play with enums
  write_line("Int value of var4 is " + to_string(var4));
  var4 = OPT_2;
  write_line("Int value of var4 is " + to_string(var4));

  // Play with unions
  write_line("Int value of var5 is " + to_string(var5.int_val));

  var5.long_val = 123456;
  write_line("Long value of var5 is " + to_string(var5.long_val));

  var5.dbl_val = 3.1415;
  write_line("Double value of var5 is " + to_string(var5.dbl_val));

  return 0;
}