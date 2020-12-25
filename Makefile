orig:
	cd bin && make main

orig-test:
	cd bin && make test

self:
	cd self && make main

self-test:
	cd self && make test

selfself:
	cd selfself && make main

selfself-test:
	cd selfself && make test

.PHONY: orig orig-test self self-test selfself selfself-test
