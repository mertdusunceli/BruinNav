#ifndef mymap
#define mymap

// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class Node
{
public:
    
    Node()
    :left(NULL), right(NULL), firstNode(true)
    {}
    
    Node(KeyType k, ValueType v)
    :value(v), key(k), left(NULL), right(NULL), firstNode(false)
    {}
    
    Node* left;
    Node* right;
    ValueType value;
    KeyType key;
    bool firstNode;
    
};

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap()
    {
        ptr_root = new Node<KeyType,ValueType>();
        m_size = 0;
    }
    
	~MyMap()
    {
        clear();
    }
    
	void clear()
    {
        freeTree(ptr_root);
        ptr_root = nullptr;
        m_size = 0;
    }
	
    int size() const
    {
        return m_size;
    }
	
    void associate(const KeyType& key, const ValueType& value)
    {
        if (ptr_root->firstNode == true)
        {
            ptr_root->key = key;
            ptr_root->value = value;
            ptr_root->firstNode = false;
            m_size++;
            return;
        }
        
        Node<KeyType,ValueType> *temp = ptr_root;
        
        
        for (;;)
        {
            if (key == temp->key)
            {
                temp->value = value;
                return;
            }
            
            if (key < temp->key)
            {
                if (temp->left != NULL)
                {
                    temp = temp->left;
                }
                else
                {
                    temp->left = new Node<KeyType,ValueType>(key, value);
                    m_size++;
                    return;
                }
            }
            
            else if (key > temp->key)
            {
                if (temp->right != NULL)
                {
                    temp = temp->right;
                }
                else
                {
                    temp->right = new Node<KeyType,ValueType>(key, value);
                    m_size++;
                    return;
                }
            }
        }
    }

    
	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const
    {
        Node<KeyType,ValueType> *temp = ptr_root;
        
        for (;;)
        {
            if (temp == NULL)
            {
                return nullptr;
            }
            
            else if (key == temp->key)
            {
                const ValueType* ptr = &(temp->value);
                return ptr;
                
            }
            
            else if (key < temp->key)
            {
                temp = temp->left;
            }
            
            else if (key > temp->key)
            {
                temp = temp->right;
            }
        }
    }

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}
    
	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    Node<KeyType,ValueType>* ptr_root;
    int m_size;
    
    void freeTree(Node<KeyType,ValueType>* cur)
    {
        
        if (cur == nullptr)
            return;
        
        freeTree(cur->left);
        freeTree(cur->right);
        delete cur;
    }
    
   

};

#endif
