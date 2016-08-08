#ifndef __BASIC_SINGLETON_H__
#define __BASIC_SINGLETON_H__

template<typename T>
class BasicSingleton {
public:
	static T *get_InstancePtr() {
		if (m_instance == nullptr) {
			m_instance = new T;
		}
		return m_instance;
	};

	static T &Inst() {
		if (m_instance == nullptr) {
			m_instance = new T;
		}
		return *m_instance;
	};

	static void release_Instance() {
		if (m_instance) {
			BasicSingleton<T>* instance = m_instance;
			m_instance = nullptr;
			delete m_instance;
		}
	};

private:
	static T *m_instance;

protected:
	/**
	 * @brief Constructor for the class BasicSingleton
	 */
	BasicSingleton() {};

	virtual ~BasicSingleton() { release_Instance(); };

};

template<typename T>
T *BasicSingleton<T>::m_instance = 0;


#endif //__BASIC_SINGLETON_H__
