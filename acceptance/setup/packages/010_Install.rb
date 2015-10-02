require 'puppet/acceptance/install_utils'
extend Puppet::Acceptance::InstallUtils
require 'beaker/dsl/install_utils'
extend Beaker::DSL::InstallUtils

test_name "Install Packages"
sha = ENV['SHA']
unless (sha) then
  fail('SHA environment variable is not set, I don\'t know what to test!')
end

step "Install repositories on target machines..." do

  repo_configs_dir = 'repo_configs'
  logger.debug('about to install repo for puppet-agent from ' + sha.to_s + ' ' + repo_configs_dir.to_s)
  agents.each do |agent|
    install_repos_on(agent, 'puppet-agent', sha, repo_configs_dir)
  end
end

PACKAGES = {
  :redhat => [
    'puppet'
  ],
  :debian => [
    'puppet'
  ],
#  :solaris => [
#    'puppet',
#  ],
#  :windows => [
#    'puppet',
#  ],
}

install_packages_on(agents, PACKAGES)

step 'Install MSIs on any Windows agents'
agents.each do |agent|
  if agent.platform.start_with?('windows')
    logger.info "Installing Puppet agent msi #{sha} on #{agent}"
    install_puppet_agent_dev_repo_on(agent, :version => sha)
    logger.info 'Prevent Puppet Service from Running'
    on(agent, puppet('resource service puppet ensure=stopped enable=false'))
    logger.info 'Vendored Ruby needs to be on PATH for pxp-agent to load libraries'
    on(agent, 'echo "export PATH=\$PATH\":/cygdrive/c/Program Files/Puppet Labs/Puppet/sys/ruby/bin\"" >> ~/.bashrc')
  end
end
