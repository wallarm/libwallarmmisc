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
        'centos7'
    ],
    target_repos: [
        'wallarm-node': [
		release: '2.14',
		dev: true
	]
    ],
    notify: [
        email: 'node-dev@wallarm.com'
    ]
)
