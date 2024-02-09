template<typename T, size_t bufferSize>
class RingBuffer {
public:
    RingBuffer() : head(0), tail(0), full(false) {}

    bool push(const T &value) {
        if (full) {
            return false; // Buffer is full, cannot push new element
        }
        buffer[head] = value;
        head = (head + 1) % bufferSize;
        full = head == tail;
        return true;
    }

    bool pop(T &value) {
        if (isEmpty()) {
            return false; // Buffer is empty, cannot pop
        }
        value = buffer[tail];
        tail = (tail + 1) % bufferSize;
        full = false;
        return true;
    }

    bool isEmpty() const {
        return (!full && (head == tail));
    }

    bool isFull() const {
        return full;
    }

    void clear() {
        head = tail;
        full = false;
    }

    size_t size() const {
        if (full) {
            return bufferSize;
        }
        if (head >= tail) {
            return head - tail;
        }
        return bufferSize + head - tail;
    }

private:
    T buffer[bufferSize];
    size_t head, tail;
    bool full;
};