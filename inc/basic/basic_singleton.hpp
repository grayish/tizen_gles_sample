#ifndef __BASIC_SINGLETON_H__
#define __BASIC_SINGLETON_H__

template<typename T>
class BasicSingleton {
public:
	/**
	 * @brief Get a pointer of an instance
	 *
	 * @return a pointer of the instance
	 */
	static T *get_InstancePtr() {
		if (m_instance == nullptr) {
			m_instance = new T;
		}
		return m_instance;
	};

	/**
	 * @brief Get an instance
	 *
	 * @return an instance
	 */
	static T &Inst() {
		if (m_instance == nullptr) {
			m_instance = new T;
		}
		return *m_instance;
	};

	/**
	 * @brief Release an instance
	 */
	static void release_Instance() {
		if (m_instance) {
			BasicSingleton<T>* instance = m_instance;
			m_instance = nullptr;
			delete instance;
		}
	};

private:
	static T *m_instance;

protected:
	/**
	 * @brief Constructor for the class BasicSingleton
	 */
	BasicSingleton() {};

	/**
	 * @brief Destructor for the class BasicSingleton
	 */
	virtual ~BasicSingleton() { m_instance = nullptr; };

};

template<typename T>
T *BasicSingleton<T>::m_instance = 0;


#endif //__BASIC_SINGLETON_H__
