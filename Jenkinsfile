#!groovy

BuildPackage(
    sources: [
        original: 'all'
    ],
    dists: [
        'jessie',
        'stretch',
        'buster',
        'trusty',
        'xenial',
        'bionic',
        'centos6',
        'centos7',
        'centos8'
    ],
    target_repos: [
        'wallarm-node': [
		release: '2.15',
		dev: true
	]
    ],
    notify: [
        email: 'node-dev@wallarm.com'
    ]
)
