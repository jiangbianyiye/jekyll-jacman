C++正则表达式标准库

### 简介

所有正则表达式相关的操作都可以归为对以下四种对象的操作：

- Target sequence(目标序列)。C风格的字符串或std::string

- Pattern(模式)。即正则表达式。类型为std::basic_regex。

- Matched arry(匹配结果)。相关的匹配结果信息，类型为std::match_results.

  std::match_results有两个常用的类型。

  ①`std::cmatch` （`std::match_results<const char*>`） 即C风格的字符串的结果集

  ②`std::smatc`   (`std::match_results<std::string::const_iterator> `)即std::string风格的结果集

- Replacement string(替换字符串)。要替换进的字符串。

### 操作库函数

对正则表达式惊醒操作的库函数有三种

#### regex_match

尝试在整个字符序列上匹配正则表达式

使用示例：

```c++
#include<iostream>
#include<regex>
#include<string>

using namespace std;

int main()
{
	//简单匹配
	string fileNames[] = { "foo.txt", "bar.txt", "baz.dat", "zoidberg" };
	regex txt_regex("[a-z]+\\.txt");
	for (const auto &fileName : fileNames) {
        //判断是整个序列是否能与正则表达式匹配
		cout << fileName << ":" << regex_match(fileName, txt_regex) << endl;
	}

	//提取子串，需要在正则表达式种用'()'指明分组
	regex base_regex("([a-z]+)\\.txt");
	smatch base_match;//匹配结果
	for (const auto & fileName : fileNames) {
		if (regex_match(fileName, base_match, base_regex)) {
			// The first sub_match is the whole string; 
			// the next sub_match is the first parenthesized expression.
			cout << fileName << " has a base of " << base_match[1].str() << endl;
		}
	}

   //提取多个子串
	regex pieces_regex("([a-z]+)\\.([a-z]+)");
	smatch pieces_match;
	for (const auto &fileName : fileNames) {
		if (regex_match(fileName, pieces_match, pieces_regex)) {
			std::cout << fileName << endl;
			for (size_t i = 0; i < pieces_match.size(); ++i) {
				std::cout << "\tsubmatch " << i << ": "
                    << pieces_match[i].str() << endl;
			}
		}	
	}
	cin.get();
	return 0;
}
```

输出：

```
foo.txt:1
bar.txt:1
baz.dat:0
zoidberg:0
foo.txt has a base of foo
bar.txt has a base of bar
foo.txt
        submatch 0: foo.txt
        submatch 1: foo
        submatch 2: txt
bar.txt
        submatch 0: bar.txt
        submatch 1: bar
        submatch 2: txt
baz.dat
        submatch 0: baz.dat
        submatch 1: baz
        submatch 2: dat
```

#### regex_search

尝试用正则表达式匹配字符序列的一部分

```c++
#include <iostream>
#include <string>
#include <regex>

using namespace std;
int main()
{
	string lines[] = { "Roses are #ff0000(red)",
						"violets are #0000ff(blue)",
						"all of my base are belong to you" };
	regex color_regex("#([a-f0-9]{2})([a-f0-9]{2})([a-f0-9]{2})");

	//简单匹配
	for (const auto & line : lines) {
		cout << line <<": "<< boolalpha << regex_search(line, color_regex) << endl;
	}
	cout << endl;

	//显示每个匹配结果中的子序列
	smatch color_match;
	for (const auto & line : lines) {
		if (regex_search(line, color_match, color_regex)) {
			cout << "matches for '" << line << endl;
			//前缀
			cout << "Prefix: '" << color_match.prefix() <<"'"<< endl;
			//匹配的各部分分组
			for (size_t i = 0; i < color_match.size(); ++i) {
				cout << i << ": " << color_match[i] << endl;
			}
			//后缀
			cout << "Suffix: '" << color_match.suffix() << "'\n" << endl;
		}
	}
	
	//重复搜索
	string log(R"(
		Speed:	366
		Mass:	35
		Speed:	378
		Mass:	32
		Speed:	400
		Mass:	30)");
	regex r(R"(Speed:\t\d*)");
	smatch sm;
	while (regex_search(log, sm, r)) {
		cout << sm.str() << '\n';
		log = sm.suffix();
	}

	//C风格字符串示例
	cmatch cm;
	if (regex_search("a test", cm, regex("test"))) {
		cout << "found" << cm[0] << "at position" << cm.prefix().length();
	}

	cin.get();
	return 0;
}
```

输出：

```
Roses are #ff0000(red): true
violets are #0000ff(blue): true
all of my base are belong to you: false

matches for 'Roses are #ff0000(red)
Prefix: 'Roses are '
0: #ff0000
1: ff
2: 00
3: 00
Suffix: '(red)'

matches for 'violets are #0000ff(blue)
Prefix: 'violets are '
0: #0000ff
1: 00
2: 00
3: ff
Suffix: '(blue)'

Speed:  366
Speed:  378
Speed:  400
foundtestat position2
```

#### regex_replace

替换匹配正则表达式的部分

示例：

```c++
#include<iostream>
#include<iterator>
#include<regex>
#include<string>

using namespace std;

int main()
{
	string text = "Quick brown fox";
	regex vowel_regex("a|e|i|o|u");

	//将结果写入输出迭代器
	regex_replace(ostreambuf_iterator<char>(std::cout), text.begin(), text.end(), vowel_regex, "*");

	cout << endl;

	//构造一个保存结果的字符串,$&表示匹配正则的字符串序列
	cout << regex_replace(text, vowel_regex, "[$&]") << endl;

	cin.get();
	return 0;
}
```

输出：

```
Q**ck br*wn f*x
Q[u][i]ck br[o]wn f[o]x
```

### 迭代器

#### regex_iterator

遍历字符序列中和正则表达式匹配的所有序列

示例：

```c++
#include<regex>
#include<string>
#include<iostream>
  
using namespace std;

int main()
{
	string s = "Quick brown fox.";

	regex words_regex("[^\\s]+");//非空白符序列
	auto words_begin = sregex_iterator(s.begin(), s.end(), words_regex);
	auto words_end = sregex_iterator();

	cout << "Found "
		<< std::distance(words_begin, words_end)
		<< " words:"<<endl;

	for (sregex_iterator i = words_begin; i != words_end; ++i) {
		cout << i->str() << endl;
	}	
	cin.get();
	return 0;
}
```

输出：

```
Found 3 words:
Quick
brown
fox.
```

#### regex_token_iterator

在所有匹配或者不匹配子序列中遍历特定的子序列

示例：

```c++
#include <iostream>
#include<string>
#include<regex>

using namespace std;

int main()
{
	string text = "Quick brown fox.";

	//tokenization （非匹配部分），可用于分割字符串
	//该迭代器为后缀迭代器
	regex  ws_regex("\\s+");//空白符,在此视作分割符
	copy(sregex_token_iterator(text.begin(), text.end(), ws_regex, -1),
		sregex_token_iterator(),
		ostream_iterator<std::string>(std::cout, "\n"));

	//iterating the first submatches
	string html = "<p><a href=\"http://google.com\">google</a> "
		"< a HREF =\"http://cppreference.com\">cppreference</a>\n</p>";
	regex url_regex("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", regex::icase);
	sregex_token_iterator end;
	for (sregex_token_iterator it(html.begin(), html.end(), url_regex, 1); it != end ; ++it) {
		cout << *it << endl;
	}
	cin.get();
	return 0;
}
```

输出：

```
Quick
brown
fox.
http://google.com
http://cppreference.com
```

参考：

\[1\] [Regular expressions library](http://en.cppreference.com/w/cpp/regex)

\[2\] [正则表达式 (C++)](https://msdn.microsoft.com/zh-cn/library/bb982727.aspx)

