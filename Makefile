orig:
	cd bin && make teal

orig-test:
	cd bin && make test

self:
	cd self && make teal


self-test:
	cd self && make test

.PHONY: orig orig-test self self-test
