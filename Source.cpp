#include<iostream>
#include<fstream>
#include <string>
using namespace std;

void strTokenOne(string& line,string**& word,int& maxSize,int& currSize)
{
	int l_size = line.size();
	maxSize = 0;
	for (int i = 0; i < l_size; i++)
	{
		if (line[i] == ' ')
		{
			maxSize++;
		}
	}
	maxSize++;
	word = new string * [maxSize];
	int j = 0;
	int i = 0;
	currSize=0;
	for (; i < maxSize;i++)
	{
		string word_term;
		for (; line[j] != ' ' && j != l_size; j++)
		{
			word_term.push_back(line[j]);
		}
		bool flag = false;
		for (int k = 0; k < currSize; k++)
		{
			if (*(word[k]) == word_term)
			{
				flag = true;
				break;
			}
		}
		
		if (flag != true)
		{
			word[currSize] = new string;
			*(word[currSize]) = word_term;
			currSize++;
		}
		if(line[j]==' ')
		j++;
	}
}

template<typename T>
class List
{
	struct Node
	{
		Node* next;
		Node* prev;
		T data;
		Node()
		{
			next = prev = 0; 
		}
		Node(Node n, Node p, T d)
		{
			next = n;
			prev = p;
			data = d;
		}
		
	} *head, * tail; int size;

	class listIterator
	{
		Node* curr;
	public:
		listIterator()
		{
			curr = 0;
		}

		listIterator(Node* ptr = 0)
		{
			curr = ptr;
		}

		listIterator& operator ++()
		{
			if (curr)
			{
				curr = curr->next;
			}
			return *this;
		}

		listIterator operator ++(int)
		{
			if (curr)
			{
				listIterator itr(curr);
				++(*this);
				return itr;
			}
			return *this;
		}

		listIterator& operator--()
		{
			if (curr)
			{
				curr = curr->previous;
			}
			return *this;
		}

		listIterator operator--(int)
		{
			if (curr)
			{
				listIterator itr(curr);
				--(*this);
				return itr;
			}
			return *this;
		}

		T& operator*()
		{
			return curr->data;
		}

		bool operator==(Node* n)
		{
			return curr == n;
		}

		bool operator!=(Node* n)
		{
			return curr!=n;
		}
	};
public:

	typedef listIterator iterator;

	listIterator begin()
	{
		listIterator i(head);

		return i;
	}

	listIterator end()
	{
		listIterator i(tail);

		return i;
	}

	List()
	{
		head = tail = 0;
		size = 0;
	}
	
	void insert(T d)
	{
		if (size == 0)
		{
			head = new Node;
			head->prev = 0;
			head->next = 0;
			head->data = d;
			tail = head;
			size++;
		}
		else
		{
			tail->next = new Node;
			tail -> next-> prev= tail;
			tail = tail->next;
			tail->next = 0;
			tail->data = d;
			size++;
		}
	}

	void deleteAtEnd()
	{
		if (head)
		{
			if (!head->next)
			{
				delete tail;
				tail = nullptr;
				head = nullptr;
				size--;
			}
			else
			{
				Node* temp = tail;
				tail = tail->prev;
				tail->next = 0;
				delete temp;
				size--;
			}
		}
	}
	
	void print()
	{
		for (listIterator l1(head); l1 != 0; ++l1)
		{
			cout << *l1 << " ";
		}
	}

	bool isEmpty()
	{
		return size == 0;
	}
	~List()
	{
		while (head)
		{
			Node* temp = head;
			head = head->next;
			delete temp;
		}
	}
};

class searchEngine
{
	class enteredDoc
	{
		string docName;
		int col_freq;
		int query_freq;
	public:
		enteredDoc(string doc)
		{
			docName = doc;
			col_freq = 0;
			query_freq = 0;
		}
		void addCol_freq(int i)
		{
			col_freq=col_freq+i;
		}
		void addQuery_freq()
		{
			query_freq++;
		}
		bool operator ==(string d)
		{
			return docName == d;
		}
		bool operator >(enteredDoc d)
		{
			if (query_freq > d.query_freq)
			{
				return true;
			}
			else if (query_freq == d.query_freq && col_freq > d.col_freq)
			{
				return true;
			}
			else if (query_freq == d.query_freq && col_freq == d.col_freq && docName < d.docName)
			{
				return true;
			}
			return false;
		}
		friend ostream& operator <<(ostream& out, const enteredDoc d);
		friend class searchEngine;
	};
	friend ostream& operator <<(ostream& out, const enteredDoc d)
	{
		out << d.docName << " " << d.query_freq << " " << d.col_freq << endl;
		return out;
	}
	class docInfo
	{
		friend class termInfo;
		friend class searchEngine;
		string DocID;
		int frequency;
	public:
		docInfo()
		{
			DocID = '\0';
			frequency = 0;
		}
		docInfo(string d, int f)
		{
			DocID = d;
			frequency = f;
		}
		friend ostream& operator <<(ostream& out, const docInfo d);
	};
	friend ostream& operator <<(ostream& out, const docInfo d)
	{
		out << d.DocID << " " << d.frequency;
		return out;
	}
	class termInfo
	{
		string key;
		List<docInfo> l1;
	public:
		termInfo(string d)
		{
			key = d;
		}

		void insert(string d)
		{
			bool flag = false;
			for (List<docInfo>::iterator i = begin(l1); i != 0; ++i)
			{
				if ((*i).DocID == d)
				{
					flag = true;
					(*i).frequency++;
					break;
				}
			}
			if (flag == false)
			{
				docInfo doc(d, 1);
				l1.insert(doc);
			}
		}

		void print()
		{
			cout << key << endl;
			l1.print();
		}

		bool operator ==(string k)
		{
			return k == key;
		}
		
		friend class searchEngine;
	}**terms; int currSize, maxSize;
public:
	searchEngine()
	{
		terms = new termInfo*[2];
		currSize = 0;
		maxSize = 2;
	}
	
	termInfo* found(string query)
	{
		int i = 0;
		while (i < currSize)
		{
			if (terms[i]->key == query)
			{
				return terms[i];
			}
			i++;
		}
		return 0;
	}

	void insertTerm(string word)
	{
		if (currSize < maxSize)
		{
			terms[currSize++] = new termInfo(word);
			
		}
		else
		{
			termInfo** temp = new termInfo * [maxSize * 2];
			for (int i = 0; i < currSize; i++)
			{
				temp[i] = terms[i];
			}
			delete[] terms;
			maxSize *= 2;
			terms = temp;
			terms[currSize++] = new termInfo(word);
		}
	}

	void createIndex(string* docs, int len)
	{
		ifstream fin;
		string ch;
		for (int i = 0; i < len; i++)
		{
			fin.open(docs[i] + ".txt");
			if (fin.is_open())
			{
				while (!fin.eof())
				{
					fin >> ch;
					termInfo* t = found(ch);
					if (t)
					{
						t->insert(docs[i]);
					}
					else
					{
						insertTerm(ch);
						terms[currSize - 1]->insert(docs[i]);
					}
				}
				fin.close();
			}
		}
	}
	void addDoctoIndex(string doc)
	{
		ifstream fin;
		string ch;
		fin.open(doc + ".txt");
		if (fin.is_open())
		{
			
			while (!fin.eof())
			{
				fin >> ch;
				termInfo* t = found(ch);
				if (t)
				{
					t->insert(doc);
				}
				else
				{
					insertTerm(ch);
					terms[currSize - 1]->insert(doc);
				}
			}
			fin.close();
		}
	}

	void searchDocuments(string query)
	{
		int size = query.size();
		int d_currSize = 0;
		int i = 0;
		int q_curr = 0;
		int q_max = 0;
		string** str;
		strTokenOne(query, str, q_max, q_curr);
		enteredDoc** d = new enteredDoc * [q_max];
		//updating
		for (int i = 0; i < q_curr; i++)
		{
			termInfo* term = found(*(str[i]));
			if (term)
			{
				for (List<docInfo>::iterator i(term->l1.begin()); i != 0; i++)
				{
					bool flag = false;
					for (int j = 0; j < d_currSize; j++)
					if (*(d[j])==(*i).DocID)
					{
						d[j]->addQuery_freq(); flag = true;
						d[j]->addCol_freq((*i).frequency);
					}
					if (flag == false)
					{
						d[d_currSize] = new enteredDoc((*i).DocID);
						d[d_currSize]->addCol_freq((*i).frequency);
						d[d_currSize++]->addQuery_freq();
					}
				}
			}
		}
		//sorting
		for (int i = 0; i < d_currSize; i++)
		{
			for (int j = i; j < d_currSize; j++)
			{
				if ((*(d[j])) > *((d[i])))
				{
					swap(d[i],d[j]);
				}
			}
		}
		if (d_currSize == 0)
		{
			cout << "No search results found!";
		}
		else
		{
			cout << "The search results are:\n";
			for (int i = 0; i < d_currSize; i++)
			{
				cout << (d[i]->docName) << endl;
			}
		}
		
	}
	
	void Print()
	{
		for (int i = 0; i < currSize; i++)
		{
			cout << i+1 << ".";
			terms[i]->print();
			cout << endl;
		}
	}
	~searchEngine()
	{
		if(terms)
		{
			delete[] terms;
			terms = nullptr;
		}
	}
};

int main()
{	
	system("cls");
	string docs[4] = { "doc 1","doc 2","doc 3","doc 4" };
	searchEngine s;
	s.createIndex(docs, 4);
	string doc("Doc 5");
	s.addDoctoIndex(doc);
	string query;
	cout << "Please enter your query: ";
	getline(cin, query);
	s.searchDocuments(query);
	return 0;
}


