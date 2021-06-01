package com.chillibits.ccom.reader;

import java.util.LinkedList;

// from: https://stackoverflow.com/q/5498865/9655481
public class LimitedQueue<E> extends LinkedList<E> {
    private final int limit;

    public LimitedQueue(int limit) {
        this.limit = limit;
    }

    @Override
    public boolean add(E e) {
        super.add(e);
        while (size() > limit) {
            super.remove();
        }
        return true;
    }
}
