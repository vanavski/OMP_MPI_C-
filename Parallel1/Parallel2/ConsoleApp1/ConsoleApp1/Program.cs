using System;

namespace ConsoleApp1
{
    class Program
    {
        static public void Main(string[] args)
        {
            // このコードは標準入力と標準出力を用いたサンプルコードです。
            // このコードは好きなように編集・削除してもらって構いません。
            // ---
            // This is a sample code to use stdin and stdout.
            // Edit and remove this code as you like.

            string lines;
            lines = Console.ReadLine();
            var str = GetPalindrome(lines);

            Console.WriteLine(str);
            Console.ReadKey();
        }

        private static string GetPalindrome(string str)
        {
            int start = 0;
            string palindrome = "";
            var length = str.Length;
            var array = new bool[length, length];

            var maxLength = 1;
            for (int i = 0; i < length; i++)
            {
                array[i, i] = true;
                if (palindrome.Length < 1)
                {
                    palindrome = str.Substring(i, 1);
                    maxLength = 1;
                    start = i;
                }
            }

            for (int i = 0; i < length - 1; i++)
            {
                if (str[i] == str[i + 1])
                {
                    if (palindrome.Length < 2)
                    {
                        palindrome = str.Substring(i, 2);
                        maxLength = 2;
                        start = i;
                    }
                    array[i, i + 1] = true;
                }
            }

            for (int k = 3; k <= length; k++)
            {
                for (int i = 0; i < length - k + 1; i++)
                {
                    int j = i + k - 1;

                    if (array[i + 1, j - 1] && str[i] == str[j])
                    {
                        array[i, j] = true;

                        if (k > maxLength)
                        {
                            start = i;
                            maxLength = k;
                        }
                    }
                }
            }
            return str.Substring(start, maxLength);
        }
    }
}
