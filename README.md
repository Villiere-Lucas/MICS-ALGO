# MICS-ALGO - MAXENCE'S WORK

## JOURNAL LOGS

**05/27/2024**  
First commit. The code works for any large number. It converts these numbers into base 16 (so base 2^4), and it does an addition between these 2 numbers.  
Everything is work. (Conversion + Addition)  
For example, with the numbers:  
987465132231458794651328764548798465584794651184213465879465132 &  
159815623184651238994562989465289562895689465984659846598658465

We get in base 2^4 (16):  
26680457EF2C4402A13F0D64116AE8C01790F6E0789CF2C22CCAC &  
637419000953F486101CF0C65F76264292A226321DB0855E89A1

And the sum is:  
2C9F45E7EFC1834B0240DC7077624B2440BB19439A77FB181564D

You can verify the conversion and the addition on this website:  
[https://www.inchcalculator.com/hex-calculator/](https://www.inchcalculator.com/hex-calculator/)


**05/28/2024**
Second commit. The code works for any large number. 
Now, substraction and multiplication works with base 2^4 (16).
All preliminaries functions / conversion functions works for remainderbignum. But the function itself is not aleady implemented.
The conversion available are : bignum2str / str2bignum / decimalToHex / HexToDecimal / BinaryToDecimal / DecimalToBinary

Here what I want to do :
Its very hard to convert bignum in base 16 into base 2. It was a problem.
BUT, I see that the conversion is working when the number is in decimal and in string.
So, you know what I mean. My idea was to do every conversion function, to transform the bignum in binary bignum, and do the binary euclidean algorithm.

So the step, with a hexadecimal bignum would be :
1) Hexadecimal bignum --> Hexadecimal String
2) Hexadecimal String --> Decimal String
3) Decimal String --> Binary String
4) Binary String --> Binary bignum
5) Apply Euclidean Algorithm & get the binary result of the remainder
6) Binary (result) bignum --> Binary string
7) Binary String --> Decimal String
8) Decimal String --> Hexadecimal String
9) Hexadecimal String --> Hexadecimal bignum.

AND BOOM ! Ok, its a lot of steps, but all of the functions are ready, and I am pretty sure that it will works.


**05/29/2024**
Third Commit. The code works for any large number.
Now, the bignumremainder() works using the binary euclidean division. And using the preliminaries functions already commit from the second commit.
Note that somes of functions were modified.


**05/29/2024**
Fourth Commit. The code works for any large number.
Now, the addmod() and multmod() functions are working. Just by using the add(), and mult() function with the remainderbignum() function.

**05/30/2024**
Fifth Commit. The code works for any large number.
Now, the expmod() function is working.