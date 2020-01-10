#include <iostream>
#include <cstring>

using namespace std;

/**
 * Lexicographic order for pairs.
 */
bool leq(int a1, int a2, int b1, int b2)
{
    return (a1 < b1 || (a1 == b1 && a2 <= b2));
}

/**
 * Lexicographic order for triples.
 */
bool leq(int a1, int a2, int a3, int b1, int b2, int b3)
{
    return (a1 < b1 || (a1 == b1 && leq(a2, a3, b2, b3)));
}

/**
 * Determine the maximum value in a slice of an array.
 */
int max(int *input, int start, int length)
{
    if (length < 1)
        return -1;

    int max = input[start];
    for (int i = start + 1; i < length; i++)
    {
        int v = input[i];
        max = v > max ? v : max;
    }

    return max;
}

/**
 * Stably sort indexes from src[0..n-1] to dst[0..n-1] with values in 0..K from v. A
 * constant offset of <code>vi</code> is added to indexes from src.
 */
void radixPass(int *src, int *dst, int *v, int vi,
               int n, int K, int start, int *cnt)
{

    // Inicializar contador
    for (int i = 0; i < K + 2; i++)
    {
        cnt[i] = 0;
    }

    // count occurrences
    for (int i = 0; i < n; i++)
        cnt[v[start + vi + src[i]]]++;

    // exclusive prefix sums
    for (int i = 0, sum = 0; i <= K; i++)
    {
        int t = cnt[i];
        cnt[i] = sum;
        sum += t;
    }

    // sort
    for (int i = 0; i < n; i++)
        dst[cnt[v[start + vi + src[i]]]++] = src[i];
}

/**
 * Find the suffix array SA of s[0..n-1] in {1..K}^n. require s[n] = s[n+1] = s[n+2] =
 * 0, n >= 2.
 */
int *suffixArray(int *s, int *SA, int n, int K, int start, int *cnt)
{
    int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
    int *s12 = new int[n02 + 3];
    s12[n02] = s12[n02 + 1] = s12[n02 + 2] = 0;
    int *SA12 = new int[n02 + 3];
    SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;
    int *s0 = new int[n0];
    int *SA0 = new int[n0];

    /*
    * generate positions of mod 1 and mod 2 suffixes the "+(n0-n1)" adds a dummy mod
    * 1 suffix if n%3 == 1
    */
    for (int i = 0, j = 0; i < n + (n0 - n1); i++)
    {
        if ((i % 3) != 0)
            s12[j++] = i;
    }

    // lsb radix sort the mod 1 and mod 2 triples
    radixPass(s12, SA12, s, +2, n02, K, start, cnt);
    radixPass(SA12, s12, s, +1, n02, K, start, cnt);
    radixPass(s12, SA12, s, +0, n02, K, start, cnt);

    // find lexicographic names of triples
    int name = 0, c0 = -1, c1 = -1, c2 = -1;
    for (int i = 0; i < n02; i++)
    {
        if (s[start + SA12[i]] != c0 || s[start + SA12[i] + 1] != c1 || s[start + SA12[i] + 2] != c2)
        {
            name++;
            c0 = s[start + SA12[i]];
            c1 = s[start + SA12[i] + 1];
            c2 = s[start + SA12[i] + 2];
        }

        if ((SA12[i] % 3) == 1)
        {
            // left half
            s12[SA12[i] / 3] = name;
        }
        else
        {
            // right half
            s12[SA12[i] / 3 + n0] = name;
        }
    }

    // recurse if names are not yet unique
    if (name < n02)
    {
        cnt = suffixArray(s12, SA12, n02, name, start, cnt);
        // store unique names in s12 using the suffix array
        for (int i = 0; i < n02; i++)
            s12[SA12[i]] = i + 1;
    }
    else
    {
        // generate the suffix array of s12 directly
        for (int i = 0; i < n02; i++)
            SA12[s12[i] - 1] = i;
    }

    // stably sort the mod 0 suffixes from SA12 by their first character
    for (int i = 0, j = 0; i < n02; i++)
        if (SA12[i] < n0)
            s0[j++] = 3 * SA12[i];
    radixPass(s0, SA0, s, 0, n0, K, start, cnt);

    // merge sorted SA0 suffixes and sorted SA12 suffixes
    for (int p = 0, t = n0 - n1, k = 0; k < n; k++)
    {
        // pos of current offset 12 suffix
        int i = (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2);
        // pos of current offset 0 suffix
        int j = SA0[p];

        if (SA12[t] < n0 ? leq(s[start + i], s12[SA12[t] + n0], s[start + j],
                               s12[j / 3])
                         : leq(s[start + i], s[start + i + 1], s12[SA12[t] - n0 + 1],
                               s[start + j], s[start + j + 1], s12[j / 3 + n0]))
        {
            // suffix from SA12 is smaller
            SA[k] = i;
            t++;
            if (t == n02)
            {
                // done --- only SA0 suffixes left
                for (k++; p < n0; p++, k++)
                    SA[k] = SA0[p];
            }
        }
        else
        {
            SA[k] = j;
            p++;
            if (p == n0)
            {
                // done --- only SA12 suffixes left
                for (k++; t < n02; t++, k++)
                {
                    SA[k] = (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2);
                }
            }
        }
    }

    return cnt;
}

int *buildSuffixArray(int *input, int start, int length)
{
    int alphabetSize = max(input, start, length);
    int *SA = new int[length + 3];
    int *_in = new int[length + 3];

    //Añadir a la entrada 3 carácteres nulos para preservar los 3-gramas
    for (int i = 0; i < length; i++)
    {
        _in[i] = input[i];
    }
    _in[length] = _in[length + 1] = _in[length + 2] = 0;

    suffixArray(_in, SA, length, alphabetSize, start, new int[alphabetSize + 2]);

    return SA;
}

int main(int argc, char const *argv[])
{
    string input = "GACCCACCACC";
    int b[input.length()];
    for (int i = 0; i < input.length(); i++)
    {
        b[i] = (int)input.at(i);
    }
    int *suffix_array = buildSuffixArray(b, 0, input.length());

    for (int i = 0; i < input.length(); i++)
    {
        cout << suffix_array[i] << endl;
    }

    return 0;
}
